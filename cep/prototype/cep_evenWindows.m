function winData = cep_evenWindows(data,winSize,overlap)
%
% function winData = cep_evenWindows(data,winSize,overlap)
%
% Windows data so there is an even number of elements in each window
%
% Imports:
%   data: raw data to be windowed (array 2*OBS)
%   winSize: Size of each window in data elements
%   overlap: proportion overlap between windows
% Exports:
%   winData: set of windowed data (array 3*winSize*numWindows)

dataSize = length(data(1,:));
overlapSize = winSize*overlap;

i = 0;
rowPosition = i*(winSize-overlapSize);
while (rowPosition+winSize <= dataSize)
    for j = 1:winSize
        if (rowPosition+j <= dataSize)
            winData(1,j,i+1) = data(1,rowPosition+j);
            winData(2,j,i+1) = data(2,rowPosition+j);
        else % if shooting of end of dataset fill with last value and appropriate dates
            winData(1,j,i+1) = 2*winData(1,j-1,i+1)-winData(1,j-2,i+1);
            winData(2,j,i+1) = winData(2,j-1,i+1);
            winData(3,j,i+1) = winData(3,j-1,i+1);
        end
    end
    i = i + 1;
    rowPosition = i*(winSize-overlapSize);
end
