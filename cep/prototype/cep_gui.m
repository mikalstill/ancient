function gui = cps_gui()
%
% function gui = cps_gui()
%
% Displays an interface to permit the viewing of geodetic data
%
% INPUTS :
%    none
% 
% OUTPUTS :
%    gui : a handle for the gui created
%

% ======================= NOTE =========================
% screen space has (0,0) at the lower left corner of the screen

set(0,'Units','pixels') ;
scnsize = get(0,'ScreenSize');

pos = [10,10,500,500];
gui_handle = figure('Units','pixels','Color',[0.8 0.8 0.8],'Position',pos,'Tag','Fig1');

% Append button
pos = [10, 10, 100, 50];
ha = uicontrol('Parent',gui_handle,'Units','pixels','Position',pos,...
    'String','Append','Tag','Pushbutton2','TooltipString','Append data to current');