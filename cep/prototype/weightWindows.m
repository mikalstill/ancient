function winData = weightWindows(winData,winType,param)
%
% winData = function weightWindows(winData,winType)
%
% function winData = weightWindows(winData,winType,param)
%
% Imports:
%   winData: set of square data windows
%   winType: desired window type
%       1 = Hamming
%       2 = Bartlett
%       3 = Blackman
%       4 = Chebyshev
%       5 = Hann
%       6 = Kaiser
%       7 = Triang
%   param: optional parameter.  Only used with Chebyshev and
%           kaiser windows.  For chebyshev it is the sidelode
%           attenuation
% Exports:
%   winData: now weighted windowed data set

if nargin < 3; param = 0;end;

[a,winSize,numWindows] = size(winData) ;

switch(winType)
case 1
    weight = hamming(winSize);
case 2
    weight = bartlett(winSize);
case 3
    weight = blackman(winSize);
case 4
    weight = chebwin(winSize,param);
case 5
    weight = hann(winSize);
case 6
    weight = kaiser(winSize,param);
case 7
    weight = triang(winSize);
otherwise
    error = 1
end
    
    
for i = 1:numWindows
    for j = 1:winSize
        winData(2,j,i) = winData(2,j,i)*weight(j);
    end
end