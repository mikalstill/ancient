function [fftdata] = fourier(time, data)
% function [fftdata] = fourier(time, data)
%
% Imports:
%   time: Array of time points
%   data: Array of data points (evenly spaced in time)
% Exports:
%   fftdata: transformed data

fftdata = fft(data);
