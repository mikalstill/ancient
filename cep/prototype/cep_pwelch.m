function [fftData, psdData, freqData] = ...
    cep_pwelch(data,interpMethod,stepSize,winType,...
    winOverlap,winSize)
%
% function [fftData, psdData, freqData] = ...
%    cep_pwelch(data,interpMethod,sampleRate,winType,...
%    winOverlap,winSize)
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
%   winOverlap: Proportion overlap between windows.
%       0   = No overlap
%       0.5 = 50% overlap
%   winSize: Number of samples in a given window

% ---- Step 1: Interpolate to get even data sampling rate ----
evenData = interpolate(data, interpMethod, sampleRate,WinSize);

% ---- Step 2: Divde into windows ----
winData = evenWindow(evenData,winSize

% ---- Step 3: Weight windows ----

% ---- Step 4: Get FFTs for each window ---

% ---- Step 5: Calculate PSDs for each window ----

% ---- Step 6: Average the PSD's ----

% ---- Step 7: Scale to required frequency