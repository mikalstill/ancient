function pWelchData = cep_pwelchAverage(psdData)
%
% function pWelchData = cep_pwelchAverage(psdData)
%
% Averages a set of psd windows to get the pWelch
%
% Imports:
%   psdData: set of windows of psd data (winSize*numWindows)
% Exports:
%   pWelchData: average of all the input windows

[winSize,numWindows] = size(psdData);

for j = 1:winSize
    pWelchData(j) = 0;
end

for i = 1:numWindows
    
    for j = 1:winSize
        pWelchData(j) = pWelchData(j) + psdData(j,i);
    end
end

for j = 1:winSize
    pWelchData(j) = pWelchData(j)/numWindows;
end
