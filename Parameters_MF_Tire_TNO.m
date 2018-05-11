% In this model a simple version of MF-Tire model is used, which needs less
% parameters. The parameters are from the "TNO MF-Tire/MF-Swift 6.1 Tire
% Model Parameters" in [1].
%
% For a complex tire model please refer to [1].
%
% The structure variable "tireparam" is used for the convenience of passing
% parameters to S-Function.
% 
% [1]Pacejka, Hans. Tire and vehicle dynamics. Elsevier, 2005.

%% Tire Paramerters Overall
tire.Fzo=4000
tire.Vo=16.7
tire.Ro=0.3135

tireparam.ov=[tire.Fzo,tire.Vo,tire.Ro];

%% Fx pure, longitudinal coefficients pure slip.
tire.pcx1=1.579
tire.pdx1=1.0422
tire.pkx1=21.687
tire.pkx2=13.728
tire.pkx3=-0.4098
tire.pex1=0.11113
tire.pex2=0.3143
tire.pex3=0
tire.Shx=0
tire.Svx=0

tireparam.fxp=[tire.pcx1 tire.pdx1 tire.pkx1 tire.pkx2 tire.pkx3...
    tire.pex1 tire.pex2 tire.pex3 tire.Shx tire.Svx];
    
%% Fy pure, lateral coefficients pure slip.
tire.pcy1=1.338
tire.pdy1=0.8785
tire.pdy2=-0.06452
tire.pky1=-15.324
tire.pky2=1.715
tire.pky4=2.00005
tire.pey1=-0.8057
tire.pey2=-0.6046
tire.Shy=0
tire.Svy=0

tireparam.fyp=[tire.pcy1 tire.pdy1 tire.pdy2 tire.pky1 tire.pky2 tire.pky4...
    tire.pey1 tire.pey2 tire.Shy tire.Svy];
    
%% Fx combined, longitudinal coefficients combined slip.
tire.rbx1=13.046
tire.rbx2=9.718
tire.rcx1=0.9995 
tire.rex1=-0.4403
tire.rex2=-0.4663
tire.Shxa=0

tireparam.fxc=[tire.rbx1 tire.rbx2 tire.rcx1 tire.rex1 tire.rex2 tire.Shxa];

%% Fy combined, lateral coefficients combined slip.
tire.rby1=10.622
tire.rby2=7.82
tire.rby3=0.002037
tire.rcy1=1.0587
tire.rey1=0.3148
tire.rey2=0.004867
tire.Shyk=0
tire.Svyk=0

tireparam.fyc=[tire.rby1 tire.rby2 tire.rby3 tire.rcy1...
    tire.rey1 tire.rey2 tire.Shyk tire.Svyk];

%% Rolling resistance Moment, rolling coefficients
tire.qsy1 = 0.00702
tire.qsy2 = 0
tire.qsy3 = 0.001515
tire.qsy4 = 0.00008514
tire.qsy7 = 0.9008

tireparam.mroll=[tire.qsy1 tire.qsy2 tire.qsy3 tire.qsy4 tire.qsy7];