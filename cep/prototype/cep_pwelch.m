function [fftData, psdData, freqScale, pWelchData] = ...
    cep_pwelch(data,interpMethod,sampleRate,winType,...
    winOverlap,winSize,valueSelection,errorSelection)
%
% function [fftData, psdData, freqData] = ...
%    cep_pwelch(data,interpMethod,sampleRate,winType,...
%    winOverlap,winSize)
% function [fftData, psdData, freqData] = ...
%    cep_pwelch(data,interpMethod,sampleRate,winType,...
%    winOverlap,winSize,valueSelection,errorSelection)
%
% Takes a random geo dataset and outputs a pwelch power 
% spectrum density.
%
% Imports:
%   data: A geodata set as readin by loadfile.m (3*OBS)
%   interpMethod:       
%      'nearest'  - nearest neighbor interpolation
%      'linear'   - linear interpolation
%      'spline'   - piecewise cubic spline interpolation (SPLINE)
%      'pchip'    - piecewise cubic Hermite interpolation (PCHIP)
%      'cubic'    - same as 'pchip'
%      'v5cubic'  - the cubic interpolation from MATLAB 5, which does not
%                   extrapolate and uses 'spline' if X is not equally spaced.
%   sampleRate: The desired sample rate in decimal years (function will 
%               interpolate the data to fill an gaps and make sure the 
%               sample rate is standardised
%   winType: Type of window weighing to use
%       1 = Square windows
%       2 = Hamming windows
%       3 = Bartlett
%       4 = Blackman
%       5 = Chebyshev
%       6 = Hann
%       7 = Kaiser
%       8 = Triangular
%   winOverlap: Proportion overlap between windows. eg:
%       0   = No overlap
%       0.5 = 50% overlap
%   winSize: Number of samples in a given window
%   valueSelection: (optional) selects the way that repeated date values
%           are resolved: Default = averge value
%   errorSelection: (option) selects the way that repeated data values are
%           resolved: Default = largest error
%           Constant values for valueSelection and errorSelection
%       1 = Average: values on the same day are averaged 
%       2 = First: uses the earliest value in the datset
%       3 = Last: uses the last value in the dataset
%       4 = Smallest error: chooses the value with the lowest error
%       5 = Largest error: chooses the value with the biggest error
% Exports:
%   fftData: Raw fourier transform data (not ready for display).  To
%           be used for inverse fourier transforms
%   psdData: Power spectral density data ready to graph
%   freqData: At set of frequency points to use as the side of the graph 
%   pWelchData: The pWelch averge set of data

% set defaults for optional parameters
if nargin < 8; errorSelection = 5; end;
if nargin < 7; valueSelection = 1; end;

% ---- Step 1: Interpolate to get even data sampling rate ----
data = cep_resolveRepeats(data, errorSelection,valueSelection);
evenData = interpolate(data, interpMethod, sampleRate,winSize,winOverlap);

% ---- Step 2: Divde into windows ----
winData = cep_evenWindows(evenData,winSize,winOverlap);

% ---- Step 3: Weight windows ----
winData = weightWindows(winData,winType);

% ---- Step 4: Get FFTs and PSDs for each window ---
[fftData,psdData,freqScale] = fftWin(winData);

% ---- Step 5: Average the PSD's to get pWelch----
pWelchData = cep_pwelchAverage(psdData);
