function [fourierData,window] = shortTimeTransform(data,beginDate,endDate,...
                                            windowingType,falloff)

% function [fourierData,window] = shortTimeTransform(data,beginDate,
%                                           endDate,windowingType,falloff)
%
% Takes a single window of data and fourier transforms it ready
% to do time domain analysis
%
% Imports:
%       data: The set of data to be transformed (3*OBS)
%       beginDate: The date of the start of the window
%       endDate: The date of the end of the window
%       windowingType: 1=square, 2=hamming, 3=chebyshev
%       falloff: Only required if windowType = chebyshev. Sets the
%                dB fall off 
% Exports:
%       fourierData:   transformed data
%       window: the data that was just transformed

% Set up default input arguments
if nargin < 5,falloff = 1; end


% constants
SQUARE = 1;
HAM = 2;
CHEB = 3;

% initialise position counter
i = 1;

% find start of requested window
while (data(1,i) < beginDate)
    i = i + 1;
end

% saves the offset of the beginning of the window
startWindow = i-1;

% get contents of window
while (data(1,i) < endDate)
    window(1,i-startWindow) = data(1,i);
    window(2,i-startWindow) = data(2,i);
    window(3,i-startWindow) = data(3,i);
    i = i + 1;
end

% get the number of elements in the window
winSize = length(window(1,:));

% if using a hamming window
if (windowingType == HAM)
    hamWin = hamming(winSize);
    for i = 1:winSize
        window(2,i) = window(2,i) * hamWin(i);
        window(3,i) = window(3,i) * hamWin(i);
    end
end

% if using Chebyshev window
if (windowingType == CHEB)
    chebyWin = chebWin(winSize,falloff);
    for i = 1:winSize
        window(2,i) = window(2,i) * chebyWin(i);
        window(3,i) = window(3,i) * chebyWin(i);
    end
end

% transform data
fourierData = fft(window(2,:));