function [value,weight] = ham(data,startWindow,winSize)
% function [value,weight] = ham(data,startWindow,winSize)
%
% gets a single hamming window value
% imports:
%   data: a data element from the standard array of data [date,value,error]
%   startWindow: the date of the start of the hamming window
%   winSize: the size (in decimal years) of the window
% exports:
%   value: the hamming value
%   weight: the weight of the value within the set

weight = 0.54 - 0.46*cos(2*pi*((data(1)-startWindow)/(winSize)));
value  = data(2) * weight;