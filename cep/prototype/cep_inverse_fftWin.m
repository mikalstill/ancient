function [winData] = cep_inverse_fftWin(fftData,sampleRate,startDate,winOverlap)
%
% function [winData] = cep_inverse_fftWin(fftData,sampleRate,winOverlap)
%
% Takes an array of fft data sets and inverse transforms them
% The new time scale is generated from the sample rate and 
% the date of the first element
%
% Imports:
%   fftData: frequency domain data sets (fftWindowSize*numWindows)
%   sampleRate: The sample rate of the time domain data
%   startDate: Date of the first element of the time domain data
%   winOverlap: Proportion overlap between windows (0 <= overlap < 1)
% Export:
%   winData: windowed set of time domain data



[fftWindowSize,numWindows] = size(fftData)

for i=1:numWindows
    dataWindow = ifft(fftData(:,i));
    winSize = length(dataWindow);
    
    for j=1:winSize
        % add time scale
        winData(1,j,i) = startDate+(j-1)*sampleRate;
        % add data
        winData(2,j,i) = real(dataWindow(j));
    end
    startDate = startDate + winSize*(1-winOverlap)*sampleRate;
end
