% For the description of the vehicle dynamics a two track model - more
% precisely, a four wheel model - is used. The roll, pitch and lift are 
% considered.
% 
% This model works well for the purpose of developing the vehicle handling
% control systems, like ESC, trajectory tracking, parking assistant and so
% on. It behaves nonlinear, simulates fast, has no sigularity. So it's a
% good choice for your first validation of controller.
% 
% The modelling of brake, drivetrain and suspension geometry are not
% included. So this model is not purposed for the development of drivability
% and comfort control systems. 

%% Parameters of chassis
chassis.lf=1.14
chassis.lr=1.50
chassis.tf=1.54
chassis.tr=1.54
chassis.m=1600
chassis.jwheel=20
chassis.h=0.6
chassis.jz=1800
chassis.rwheel=tire.Ro

chassis.jy=1700
chassis.jx=450
chassis.kf=51000
chassis.df=2250
chassis.kr=46500
chassis.dr=2000

%% Parameters for controller
chassis_virtual=chassis

%% Parameters in reality
chassis_real=chassis

%%
clear chassis