function data2 = interpolate(data1, method, sampleRate, winSize,overlap)
% function data2 = interpolate(data1, method, sampleRate, winSize)
%
% Interpolates a set of CEP data
%
% Imports:
%   Data1: 2xOBS set of data (dates and values)
%   method: string value one of: 
%       'nearest' Nearest neighbor interpolation
%       'linear'  Linear interpolation (default)
%       'spline'  Cubic spline interpolation
%       'pchip'   Piecewise cubic Hermite interpolation
%   sampleRate: Amount of time between samples in interpolated set
%   winSize: Optional parameter, will extrapolate the set to make it fill
%       a set of windows without half empty windows being generated.
%       winSize is in number of samples.  winSize*sampleRate = winLength in time
%   overlap: Also optional for the same reson (overlap in desired later 
%       windows as a proportion: 0.5 = 50%, 0 = No overlap)
% Exports:
%   Data2: output dataset of evenly spaced data points

% set 
if nargin < 5
    overlap = 0;
end
if nargin < 4
    winSize = 1;
end

len1 = length(data1(1,:));

%intermediate variables to simplify equation 
Q = ((data1(1,len1)-data1(1,1))/sampleRate)-winSize;
R = winSize*(1-overlap);

% generate new time set
B = data1(1,1):sampleRate:data1(1,len1)-((mod(Q,R)+1)*sampleRate);

% interpolate data to get data points
A = interp1(data1(1,:),data1(2,:),B,method);

L = length(B);
% pre create zeroed matrix for speed
data2(2, L) = 0;

for i=1:L
    data2(1,i) = B(i);
    data2(2,i) = A(i);
end

%data2 = [B,A];
