function data2 = interpolate(data1, method, sampleRate)
% function data2 = interpolate(data1, method, sampleRate)
%
% Imports:
%   Data1: 2xOBS set of data (dates and values)
%   method: string value one of: 
%       'nearest' Nearest neighbor interpolation
%       'linear'  Linear interpolation (default)
%       'spline'  Cubic spline interpolation
%       'pchip'   Piecewise cubic Hermite interpolation
%   sampleRate: Amount of time between samples in interpolated set
% Exports:
%   Data2: output dataset of evenly spaced data points

% pre create zeroed matrix 
data2(2, floor((data1(1,length(data1(1,:)))-data1(1,1))/sampleRate)) = 0;

% generate new time set
data2(1,:) = linspace(data1(1,1):data1(1,length(data1(1,:))), length(data2(1,:)));

% interpolate data to get data points
data2(2,:) = interp1(data1(2,:),data1(1,:),data2(1,:),method,'extrap');

