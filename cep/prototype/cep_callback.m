function f = cep_callback( object )
%
% cep_callback( object )
%
% Performs the callback routines required when the user interacts with the UI
%
% PRE:
%       The UI has been displayed and the user is madly clicking on things.
% POST:
%       The UI has been updated with the results of the mad clicks.
%
% INPUTS:
%       object - The object which is the subject of the callback (eg 'Load', or 'Quit' buttons).
%                This should be a string representation of the object.
%
% OUTPUTS:
%       none.
%

% ==============================================================
switch( object )
case 'Exit'
    close;
case 'Load'
    loadFiles;
case 'Refresh'
    refreshPlots;
end



% ==============================================================
% loads the files that the user has specified
% ==============================================================
function foo = loadFiles
%
% function loadFiles
%
%
%
%
%
[name,path] = uigetfile('*.*');
filename = strcat(path,name);
l = length(filename);
filename = filename(1:l-1);
datfi1e1 = strcat(filename,'1');
datfile2 = strcat(filename,'2');
datfile3 = strcat(filename,'3');

[dataN,sizeN,header1N,header2N,header3N] = loadfile(datfi1e1);
[dataE,sizeE,header1E,header2E,header3E] = loadfile(datfile2);
[dataU,sizeU,header1U,header2U,header3U] = loadfile(datfile3);

updatePlot('North', dataN(1,:), dataN(2,:), dataN(3,:));
updatePlot('East', dataE(1,:), dataE(2,:), dataE(3,:));
updatePlot('Up', dataU(1,:),dataU(2,:), dataU(3,:));

foo = 0;



% ==============================================================
% updates the graphs
% ==============================================================
function foo = updatePlot( direction, date, data, error )
%
%
%
%

handle = findobj(gcf, 'Tag',strcat('Axis',direction));

axes(handle); 

hold on;

if nargin == 4
    handle = findobj(gcf, 'Tag',strcat('ErrBar'));
    checked = get( handle, 'Value');
    if( checked == 1 )
        errorbar(date, data, error, 'b');
    end
end
ph = plot(date,data,'m');    
PlotTitle = sprintf('Data %s',direction);
title(PlotTitle);
hold off;

foo = 0;



% ==============================================================
% updates the graphs
% ==============================================================
function foo = refreshPlots()
%
%
%
%

handle = findobj(gcf, 'Tag',strcat('AxisNorth'));


end


