function winData = weightWindows(winData,winType,param)
%
% winData = function weightWindows(winData,winType)
%
% function winData = weightWindows(winData,winType,param)
%
% Imports:
%   winData: set of square data windows
%   winType: desired window type
%       1 = Square windows
%       2 = Hamming windows
%       3 = Bartlett
%       4 = Blackman
%       5 = Chebyshev
%       6 = Hann
%       7 = Kaiser
%       8 = Triangular
%   param: optional parameter.  Only used with Chebyshev and
%           kaiser windows.  For chebyshev it is the sidelode
%           attenuation
% Exports:
%   winData: now weighted windowed data set

if nargin < 3; param = 0;end;

[a,winSize,numWindows] = size(winData) ;

switch(winType)
case 1
    weight = boxcar(winSize)
case 2
    weight = hamming(winSize);
case 3
    weight = bartlett(winSize);
case 4
    weight = blackman(winSize);
case 5
    weight = chebwin(winSize,param);
case 6
    weight = hann(winSize);
case 7
    weight = kaiser(winSize,param);
case 8
    weight = triang(winSize);
otherwise
    error = 1
end
    
    
for i = 1:numWindows
    for j = 1:winSize
        winData(2,j,i) = winData(2,j,i)*weight(j);
    end
end