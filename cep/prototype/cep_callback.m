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
    filename = get_filename;
end



% ==============================================================
%                gets the filenmame from the user
% ==============================================================
function name = get_filename
%
%
%
screen_size = get(0,'ScreenSize');

load_width=screen_size(3)/4;
load_height=screen_size(4)/4;
load_pos = [ (screen_size(3)-load_width)/2, (screen_size(4)-load_height)/2, load_width, load_height ];
load_handle = figure('Units','pixels','Color',[0.8 0.8 0.8],'Position',load_pos,'Tag','Select a File');
set(load_handle, 'WindowStyle', 'modal' );
% set(load_handle,'MenuBar','none');

sites = get_files;
dirs = get_dirs;

space = 10;
dirbox_pos = [ space, space, load_width/2-space, load_height-(2*space) ];
dirbox_handle = uicontrol('Parent',load_handle,'Units','pixels','BackgroundColor',[1.0 1.0 1.0],'Position',dirbox_pos,'String',dirs,'Style','listbox','Tag','Filebox','Value',1,'UserData',dirs);

filebox_pos = [ load_width/2+space/2, space, load_width/2-space, load_height-(2*space) ];
filebox_handle = uicontrol('Parent',load_handle,'Units','pixels','BackgroundColor',[1.0 1.0 1.0],'Position',filebox_pos,'String',sites,'Style','listbox','Tag','Dirbox','Value',1,'UserData',sites);

% return something
name='blah';



% ==============================================================
%                gets a listing of the current directory
% ==============================================================
function files = get_files
% function list = get_files
%
% gets a listing for the current working directory.
%
% this code is based on the 'tsview' from MIT.
%
% INPUTS:
%       cwd - the directory to get the listing of
% OUTPUTS
%       list - the directory listing
% PRE:
%       none
% POST:
%       none
%

% get the directory listing
listing = dir;

names = {listing.name};
% get all file names with mb_ in the first three characters
names = names(strncmp(names,'mb_',3));
% Now sort the names
names = sort(names)';

% Reduce the names further by getting only the site names
list = [];
for i = 1:length(names)
	tname = cell2struct(names(i),'name');
	if findstr(tname.name,'.dat1')
		st = tname.name;
		list = [list;st(4:11)];
	end
end
files=list;



% ==============================================================
%                gets a listing of the current directory
% ==============================================================
function dirs = get_dirs
% function list = get_dirs
%
% gets a listing of directories only for the current working
% directory.
%
% this code is based on the 'tsview' from MIT.
%
% INPUTS:
%       cwd - the directory to get the listing of
% OUTPUTS
%       list - the directory listing
% PRE:
%       none
% POST:
%       none
%

% get the directory listing
listing = dir;

% Isolate the directories
j=0; dirlist=[];
for i = 1:length(listing)
    if listing(i).isdir
        dirlist = [dirlist;listing(i)];
    end
end

%get their names
names = { dirlist.name };
list = [];
%for i = 1:length(names)
%    tname = cell2struct(names(i),'name');
%    st = tname.name;
%    % TODO... require a fixed size filename to keep name length consistency
%    list = [list;st];
%end

tname = cell2struct(names(3),'name');
st = tname.name;
list = [st];

dirs=list;
