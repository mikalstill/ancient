function [fftData,psdData,freqScale] = fftWin(winData)
%
% function [fftData,psdData,freqScale] = fftWin(winData)
%
% Get the fft and psd of a set of windows
%
% Imports:
%   winData: windowed set of date value pairs (3*winSize*numWindows)
% Exports:
%   fftData: fft of the values in each window
%   psdData: power spectral density of each window
%   freqScale: frequency base "time" scale

[a,winSize,numWindows] = size(winData);

for i=1:numWindows
    fftDataWindow = fft(winData(2,:,i));
    fftWinSize = length(fftDataWindow);
    psdWindow = fftDataWindow.*conj(fftDataWindow)/length(winData(2,:,i));

    for j = 1:fftWinSize
        fftData(j,i) = fftDataWindow(j);
        psdData(j,i) = psdWindow(j);
    end
end
