function gui = cep_gui()
%
% function gui = cep_gui()
%
% Displays an interface to permit the viewing of geodetic data
%
% PRE:
%       None.
% POST
%       The UI has been displayed.
%
% INPUTS :
%       None.
% 
% OUTPUTS :
%       gui - The handle of the 'figure' created.
%

% =========================== NOTE =============================
% screen space has (0,0) at the lower left corner of the screen
% ==============================================================

% set the units of the root window to pixels
set(0,'Units','pixels');

% ==============================================================
% get selected properties of the root window
% NOTE: to view the properties of the root window type 'get(0)'
% ==============================================================
screen_size = get(0,'ScreenSize');

border_width = 10;
top_border_width = 10;


frame_pos = [15,70,screen_size(3)-30,screen_size(4)-150];
% pos = [16 67 1120 734]
gui_handle = figure('Units','pixels','Color',[0.8 0.8 0.8],'Position',frame_pos,'Tag','Fig1');
set(gui_handle,'ToolBar','figure');

% number of buttons
button_count = 3;
x_location = 40;
button_width = 100;
button_height = 35;
check_width = 10;
button_space = (frame_pos(4) - (button_count*button_height))/(button_count+1);


% Load button
load_pos = [x_location, frame_pos(4)-button_space, button_width, button_height];
load_handle = uicontrol('Parent', gui_handle, 'Callback', 'cep_callback(''Load'')', ...
    'Units', 'pixels', 'Position', load_pos, 'String', 'Load', 'Tag', 'load_button', ...
    'TooltipString', 'Load data from file');

% Referesh button
refresh_pos = [x_location, frame_pos(4)-((2*button_space)+button_height), button_width, button_height];
refresh_handle = uicontrol('Parent', gui_handle, 'Callback', 'cep_callback(''Refresh'')', ...
    'Units', 'pixels', 'Position', refresh_pos, 'String', 'Refresh', 'Tag', 'refresh_button', ...
    'TooltipString', 'Refresh the Plots');

% Exit button
exit_pos = [x_location, frame_pos(4)-((3*button_space)+button_height), button_width, button_height];
exit_handle = uicontrol('Parent', gui_handle, 'Callback', 'cep_callback(''Exit'')', ...
    'Units', 'pixels', 'Position', exit_pos, 'String', 'Exit', 'Tag', 'exit_button', ...
    'TooltipString', 'Exit the application');

% Error bar check box
errBar_pos = [x_location, frame_pos(4)-((4*button_space)+button_height), check_width, check_width];
errBar_handle = uicontrol('Style', 'checkbox', 'Parent', gui_handle, 'String', 'Toggle Error Bars', ...
    'Units', 'pixels', 'Position', errBar_pos, 'Tag', 'errBar_check', 'TooltipString', ...
    'Toggle Error Bars');


axis_space = 60;
axis_width  = 2*screen_size(3)/3;
axis_height = screen_size(4)/4;


pos = [border_width + (screen_size(3)/5), top_border_width + (axis_space/3), axis_width, axis_height];
axis1 = axes('Parent',gui_handle, 'Units','pixels', 'CameraUpVector',[0 1 0], ...
    'Color',[1 1 1], 'Position',pos, 'Tag','AxisNorth', ...
    'XColor',[0 0 0], 'YColor',[0 0 0], 'ZColor',[0 0 0]);

pos = [border_width + (screen_size(3)/5), top_border_width + (axis_height+(2*axis_space/3)), axis_width, axis_height];
axis2 = axes('Parent',gui_handle, 'Units','pixels', 'CameraUpVector',[0 1 0], ...
    'Color',[1 1 1], 'Position',pos, 'Tag','AxisEast', ...
    'XColor',[0 0 0], 'YColor',[0 0 0], 'ZColor',[0 0 0]);

pos = [border_width + (screen_size(3)/5), top_border_width + ((2*axis_height)+(axis_space)), axis_width, axis_height];
axis3 = axes('Parent',gui_handle, 'Units','pixels', 'CameraUpVector',[0 1 0], ...
    'Color',[1 1 1], 'Position',pos, 'Tag','AxisUp', ...
    'XColor',[0 0 0], 'YColor',[0 0 0], 'ZColor',[0 0 0]);

gui = gui_handle;
