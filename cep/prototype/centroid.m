function [pointArray] = centroid(winArray,type,winSize,overlap)
% function [pointArray] = centroid(winArray,type,winSize,overlap)
%
% Finds the weighted centroid of a set of windowed data
% Imports:
%   winArray: Array of windowed data (output of windows.m)
%   type: type of centroid weighting to use 1 = square, 2 = hamming
%   winSize: Size in time of each window
%   overlap: the proportion overlap of the windows
% Exports:
%   pointArray: Array of points representing the central point of each window

% set constants
SQUARE = 1;
HAMMING = 2;


% get dimensions of winArray
[A, numWindows, largestWindow] = size(winArray);

% get start date for first window
startCurrent = winArray(1,1,1);

% for optimisation reasons
halfWinSize = winSize * 0.5;

for winNum=1:numWindows
    position = 1; % position within a window
    total = 0;
    denominator = 0;

    while ((position <= largestWindow) & ~(winArray(1,winNum,position) == 0))
        if (type == SQUARE)
            total = total + winArray(2,winNum,position);
            denominator = denominator + 1;
        elseif (type == HAMMING)
            [value, weight] = ham(winArray(:,winNum,position),startCurrent,winSize);
            total = total + value;
            denominator = denominator + weight;
        end
        position = position + 1;
    end
    
    if ~(denominator == 0)
        tempArray(1,winNum) = startCurrent + halfWinSize;
        tempArray(2,winNum) = total/denominator;
    else
        tempArray(1,winNum) = 0;
        temptArray(2,winNum) = 0;
    end
    startCurrent = startCurrent + winSize - winSize*overlap;
    
end

% delete empty rows

arraySize = length(tempArray(1,:));
j = 1;
for i=1:arraySize
    if not(tempArray(1,i) == 0)
        pointArray(1,j) = tempArray(1,i);
        pointArray(2,j) = tempArray(2,i);
        j = j + 1;
    end
end
