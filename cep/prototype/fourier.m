function [fftdata,psd] = fourier(time, data)
% function [fftdata] = fourier(time, data)
%
% Imports:
%   time: Array of time points
%   data: Array of data points (evenly spaced in time)
% Exports:
%   fftdata: transformed data
%   psd: power spectral density

fftdata = fft(data);
psd = fftdata.*conj(fftdata)/length(data);
