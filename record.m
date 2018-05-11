function [myworld,f] = recordvrml(dimension)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
worlds = vrwho;
for i=1:length(worlds)
myworld = worlds(i);
% set(myworld,'RecordMode','manual')
set(myworld,'RecordMode','scheduled');
if nargin==1
if dimension==2
    f=get(myworld,'Figures');
    set(f,'Record2D','on')
    set(f,'NavPanel','none');
%     set(myworld,'Record3D','off')
else
    set(myworld,'Record3D','on')
end
else
    set(myworld,'Record3D','on')
end
set(myworld,'RecordInterval',[0 Inf]);
end
end

