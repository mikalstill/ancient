function [winData] = cep_winUnweight(winData,winType,param)
%
% function [winData] = cep_winUnweight(winData,winType,param)
%
% Imports:
%   winData: windowed data sets (2*winSize*numWindows)
%   winType: Type of window weighting:
%       1 = Square windows
%       2 = Hamming windows
%       3 = Bartlett
%       4 = Blackman
%       5 = Chebyshev
%       6 = Hann
%       7 = Kaiser
%       8 = Triangular
%   param: Optional parameter only used with Chebyshev and
%           kaiser windows.  For chebyshev it is the sidelode
%           attenuation
% Export:
%   winData: unWeighted data sets (2*winSize*numWindows)


if nargin < 6; param = 0; end;

[a,winSize,numWindows] = size(winData);

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
        winData(2,j,i) = winData(2,j,i)*(1/weight(j));
    end
end
