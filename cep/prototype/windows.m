function [windowArray, numWindows] = windows(data,winSize,overlap)
% function [windowArray, numWindows] = windows(data,winSize,overlap)
%
% Regroups data into square windows with a width defined in time passed
% Imports:
%   data: matrix of gps data in standard CEP prototype form
%   winSize: width of each window in decimal years
%   overlap: the amount of overlap between windows
% Exports:
% windowArray: Array of windowed data (3 * numWindows * largestWindow)
%            windows with a small amount of data will be zero
%            filled, this is a limitation of matlab.
% numWindows: The number of seperate windows that were populated with data

%Get timescale of data set
numSamples = length(data(1,:));
firstdate = data(1,1);
lastdate = data(1,numSamples);

% For optimizing speed slightly
overlapWinSize = winSize * (1-overlap);

%Work out number of windows
value = (((lastdate - firstdate)*(1+2*overlap))/winSize);
numWindows = ceil(value)
if (overlap ~= 0)
  numWindows = numWindows - 1
end

%Divide into windows
nextFirstRecord = 1;
startWindow = data(1,1) - winSize;
for i=1:numWindows-1
    % point array counter to start of window
    j = nextFirstRecord;
    currentFirstRecord = nextFirstRecord;
    % save array reference to start of window
    startWindow = startWindow + overlapWinSize;
    % populate first half of window
    while data(1,j) < startWindow+overlapWinSize
        for k=1:3
            windowArray(k,i,j-(currentFirstRecord-1)) = data(k,j);
        end
        % increment array counter
        j = j + 1;
    end
    
    % mark start of next window for later use
    nextFirstRecord = j; 
    % populate second half of window
    while data(1,j) < startWindow+winSize
        for k=1:3
            windowArray(k,i,j-(currentFirstRecord-1)) = data(k,j);
        end
        % increment array counter
        j = j + 1;
    end
end

% populate final window with remaining data
j = nextFirstRecord;
currentFirstRecord = nextFirstRecord;
startWindow = startWindow + winSize;
while j < numSamples
    for k=1:3
        windowArray(k,numWindows,j-(currentFirstRecord-1)) = data(k,j);
    end
    j = j + 1;
end    