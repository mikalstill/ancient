function [data, size, header1, header2, header3] = loadfile(filename)
% function [data, size, header1, header2, header3] = loadfile(filename)
%
% Loads geo data into a matrix ready for manipulation
% Imports:
%   filename: string containing filename
% Exports:
%   data: matrix of geodata [3,OBS]
%   size: number of observations read
%   header{1,2,3} contains the header lines from the file 

[fid, message] = fopen(filename,'r');
if fid == -1
    disp(message)
    ok = 0;
    return
end	
% Now we will try to read the file.  These
% files have three header lines and then data
%
% Read the header lines
header1 = fgetl(fid); 
header2 = fgetl(fid); 
header3 = fgetl(fid);
% Read data into column vector
[rData,tot] = fscanf(fid,' %f %f %f');
% Close the data file at this point
fclose(fid); 
% Now reshape the Data into 3:tot/3 array
size = tot/3;
data = reshape(rData,3,size);

