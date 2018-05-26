/*
 * File : mf_tire.c
 * Abstract:
 *
 * Copyright 1990-2013 The MathWorks, Inc.
 */


#define S_FUNCTION_NAME  mf_tire
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define LAMBDA(element) (*LambdaPtrs[element])  
#define ALPHA(element) (*AlphaPtrs[element])  
#define FZ(element) (*FzPtrs[element])
#define MU(element) (*MuPtrs[element]) 

// Overall parameter
#define OV_IDX 0
// Fzo

// Fx pure
#define FXP_IDX 1
// pcx1
// pdx1
// pkx1
// pkx2
// pkx3
// pex1
// pex2
// pex3
// Shx
// Svx

// Fy pure
#define FYP_IDX 2
// pcy1
// pdy1
// pdy2
// pky1
// pky2
// pky4
// pey1
// pey2
// Shy
// Svy

// Fx combined
#define FXC_IDX 3
// rbx1
// rbx2
// rcx1
// rex1
// rex2
// Shxa

// Fy combined
#define FYC_IDX 4
// rby1
// rby2
// rby3
// rcy1
// rey1
// rey2
// Shyk
// Svyk
        
#define NPARAMS 5
#define IOWIDTH 4
#define NINPUTS 4
#define NOUTPUTS 2
        
/*================*
 * Build checking *
 *================*/


/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *   Setup sizes of the various vectors.
 */
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, NPARAMS);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return; /* Parameter mismatch will be reported by Simulink */
    }

//     if ( mxGetNumberOfElements(ssGetSFcnParam(S,0)) != 3 ||
//             mxGetNumberOfElements(ssGetSFcnParam(S,1)) != 10 ||
//             mxGetNumberOfElements(ssGetSFcnParam(S,2)) != 10 ||
//             mxGetNumberOfElements(ssGetSFcnParam(S,3)) != 6 ||
//             mxGetNumberOfElements(ssGetSFcnParam(S,4)) != 8 ){
//         return;
//     }
    
    if (!ssSetNumInputPorts(S, NINPUTS)) return;
    int_T iInput = 0;
    int_T nInput = ssGetNumInputPorts(S);
    for ( iInput=0; iInput < nInput; iInput++ )
    {
        ssSetInputPortWidth(S, iInput, IOWIDTH);
        ssSetInputPortDirectFeedThrough(S, iInput, 1);
    }

    if (!ssSetNumOutputPorts(S,NOUTPUTS)) return;
    int_T iOutput = 0;
    int_T nOutput = ssGetNumOutputPorts(S);
    for ( iOutput=0; iOutput < nOutput; iOutput++ )
    {
        ssSetOutputPortWidth(S, iOutput, IOWIDTH);
    }
    
    ssSetNumSampleTimes(S, 1);

    /* specify the sim state compliance to be same as a built-in block */
    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);

    ssSetOptions(S,
                 SS_OPTION_WORKS_WITH_CODE_REUSE |
                 SS_OPTION_EXCEPTION_FREE_CODE |
                 SS_OPTION_USE_TLC_WITH_ACCELERATOR);
}


/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    Specifiy that we inherit our sample time from the driving block.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S); 
}

/* Function: mdlOutputs =======================================================
 * Abstract:
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    InputRealPtrsType LambdaPtrs  = ssGetInputPortRealSignalPtrs(S,0);
    InputRealPtrsType AlphaPtrs   = ssGetInputPortRealSignalPtrs(S,1);      
    InputRealPtrsType FzPtrs      = ssGetInputPortRealSignalPtrs(S,2);
    InputRealPtrsType MuPtrs      = ssGetInputPortRealSignalPtrs(S,3);
    
    real_T            *Fxc    = ssGetOutputPortRealSignal(S,0);
    real_T            *Fyc    = ssGetOutputPortRealSignal(S,1);
    
    real_T *ov_param     = mxGetPr(ssGetSFcnParam(S,OV_IDX));
    real_T *fxp_param    = mxGetPr(ssGetSFcnParam(S,FXP_IDX));
    real_T *fyp_param    = mxGetPr(ssGetSFcnParam(S,FYP_IDX));
    real_T *fxc_param    = mxGetPr(ssGetSFcnParam(S,FXC_IDX));
    real_T *fyc_param    = mxGetPr(ssGetSFcnParam(S,FYC_IDX));
   
    real_T Fzo = ov_param[0];
    
    // Fx pure
    real_T pcx1,pdx1,pkx1,pkx2,pkx3,pex1,pex2,pex3,Shx,Svx;
        pcx1 = fxp_param[0];
        pdx1 = fxp_param[1];
        pkx1 = fxp_param[2];
        pkx2 = fxp_param[3];
        pkx3 = fxp_param[4];
        pex1 = fxp_param[5];
        pex2 = fxp_param[6];
        pex3 = fxp_param[7];
        Shx = fxp_param[8];
        Svx = fxp_param[9];

    // Fy pure
    real_T pcy1,pdy1,pdy2,pky1,pky2,pky4,pey1,pey2,Shy,Svy;  
        pcy1 = fyp_param[0];
        pdy1 = fyp_param[1];
        pdy2 = fyp_param[2];
        pky1 = fyp_param[3];
        pky2 = fyp_param[4];
        pky4 = fyp_param[5];
        pey1 = fyp_param[6];
        pey2 = fyp_param[7];
        Shy  = fyp_param[8];
        Svy  = fyp_param[9];

    // Fx combined
    real_T rbx1,rbx2,rcx1,rex1,rex2,Shxa;
        rbx1 = fxc_param[0];
        rbx2 = fxc_param[1];
        rcx1 = fxc_param[2];
        rex1 = fxc_param[3];
        rex2 = fxc_param[4];
        Shxa = fxc_param[5];
        
     // Fy combined
     real_T rby1,rby2,rby3,rcy1,rey1,rey2,Shyk,Svyk;                
        rby1 = fyc_param[0];
        rby2 = fyc_param[1];
        rby3 = fyc_param[2];
        rcy1 = fyc_param[3];
        rey1 = fyc_param[4];
        rey2 = fyc_param[5];
        Shyk = fyc_param[6];
        Svyk = fyc_param[7];       

    real_T dfz;
    real_T lambdax, Cx, Dx, Kx, Ex, Bx, Bxlx, Ax, Fxp;
    real_T alphay, Cy, Dy, Ky, Ey, By, Byay, Ay, Fyp;
    real_T alphas, Bxa, Exa, Bxaas, Axa, Gxa;
    real_T lambdas, Byk, Eyk, Bykks, Ayk , Gyk;
        
    int_T i=0, sign[4] = {1,-1,1,-1};
    for (i=0; i<IOWIDTH; i++) 
    {
        dfz = FZ(i)/Fzo-1;
   
        Cx = pcx1;
        Dx = pdx1*MU(i)*FZ(i);
        if (Dx < 0.1) Dx = 0.1;
        Kx = FZ(i)*(pkx1+pkx2*dfz)*exp(dfz*pkx3);
        Ex = pex1 + pex2*dfz + pex3*dfz*dfz;
        if (Ex > 1) Ex = 1;
        Bx = Kx/Cx/Dx;
        lambdax = LAMBDA(i) + Shx;
        Bxlx = Bx*lambdax;
        Ax = Bxlx - Ex*(Bxlx - atan(Bxlx));
        Fxp = Dx*sin(Cx*atan(Ax)) + Svx;
        
        Cy = pcy1;
        Dy = (pdy1+pdy2*dfz)*MU(i)*FZ(i);
        if (Dy < 0.1) Dy = 0.1;
        Ky = pky1*Fzo*sin(pky4*atan(FZ(i)/Fzo/pky2));
        Ey = pey1 + pey2*dfz;   
        if (Ey > 1) Ey = 1;
        By = Ky/Cy/Dy;
        alphay = ALPHA(i) + Shy*sign[i];
        Byay = By*alphay;
        Ay = Byay - Ey*(Byay - atan(Byay));
        Fyp = Dy*sin(Cy*atan(Ay)) + Svy*sign[i];
        
        Bxa = rbx1*cos(atan(rbx2*LAMBDA(i)));
        Exa = rex1 +rex2*dfz;
        if (Exa > 1) Exa = 1;
        alphas = ALPHA(i) + Shxa*sign[i];
        Bxaas = Bxa*alphas;
        Axa = Bxaas - Exa*(Bxaas - atan(Bxaas));
        Gxa = cos(rcx1*atan(Axa));
        Fxc[i] = Gxa*Fxp;
        
        Byk = rby1*cos(atan(rby2*(ALPHA(i)-rby3)));
        Eyk = rey1 + rey2*dfz;
        if (Eyk > 1) Eyk = 1;
        lambdas = LAMBDA(i) + Shyk;
        Bykks = Byk*lambdas;
        Ayk = Bykks - Eyk*(Bykks - atan(Bykks));
        Gyk = cos(rcy1*atan(Ayk));
        Fyc[i] = Gyk*Fyp + Svyk*sign[i];
    }
}


/* Function: mdlTerminate =====================================================
 * Abstract:
 *    No termination needed, but we are required to have this routine.
 */
static void mdlTerminate(SimStruct *S)
{
}



#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
