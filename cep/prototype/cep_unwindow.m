function dataSet = cep_unwindow(winData,winOverlap)
%
% function dataSet = cep_unwindow(winData,winOverlap)
%
% Turns an group of windows into a linear data set.
% Non matching data points will be averaged
%
% Imports:
%   winData: (2*winSize*numWindows)
%   winOverlap: Proportion overlap between windows

[a,winSize,numWindows] = size(winData)

winLength = floor(winSize*(1-winOverlap));
overlapLength = winSize*winOverlap;

%do first window
for i=1:winLength
    dataSet(1,i) = winData(1,i,1);
    dataSet(2,i) = winData(2,i,1);
end

%do rest of 
for i=2:numWindows
    
    for j=1:winLength
        dataSet(1,j+winLength*(i-1)) = winData(1,j,i);
        if (j <= overlapLength)
            dataSet(2,j+winLength*(i-1)) = (winData(2,j,i)+winData(2,j+winLength,i-1))*0.5;
        else
            dataSet(2,j+winLength*(i-1)) = winData(2,j,1);
        end
    end
end

%do last little bit
for i=winLength+1:winSize
    dataSet(1,i+winLength*(numWindows-1)) = winData(1,i,numWindows);
    dataSet(2,i+winLength*(numWindows-1)) = winData(2,i,numWindows);
end