function [dataSet,winData] = cep_inverse_pwelch(fftData,sampleRate,startDate,winOverlap,winType,param)
%
% function [dataSet] = cep_inverse_pwelch(fftdata,sampleRate,startDate,winOverlap,winType,param)
% function [dataSet] = cep_inverse_pwelch(fftdata,sampleRate,startDate,winOverlap,winType)
%
% Takes the fftdata sets of a pwelch set and converts them 
% back to a single time based dataset
%
% Imports:
%   fftData: set of frequency domain data sets
%   sampleRate: The sampling rate used before transformation (in decimal years)
%   winOverLap: Proportion overlap between windows (0 <= winOverLap < 1)
%   winType: type of weighting used on each window:
%       1 = Square windows
%       2 = Hamming windows
%       3 = Bartlett
%       4 = Blackman
%       5 = Chebyshev
%       6 = Hann
%       7 = Kaiser
%       8 = Triangular
%   param: Optional parameter only used with Chebyshev and
%           kaiser windows.  For chebyshev it is the sidelode
%           attenuation
% Exports:
%   dataSet: The time domain data set
%   winData: Unweighted windowed time domain data

if nargin < 6; param = 0; end;


% Step 1 - Inverse FFT and Re-attach time scale
[winData] = cep_inverse_fftWin(fftData,sampleRate,startDate,winOverlap);

% Step 2 - Unweight windows
if winType > 1
    winData = cep_winUnweight(winData,winType,param);
end

% Step 3 - Unwindow data
% In cases of overlapping values an average is used

dataSet = cep_unwindow(winData,winOverlap);