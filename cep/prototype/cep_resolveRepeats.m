function [newData] = cep_resolveRepeats(data, valueSelection, errorSelection)
%
% function [newData] = cep_resolveRepeats(data)
%
% Before interpolate can be run the list must have no repeated values
% this function searches for repeated dates and resolves them by the
% specified method
%
% Imports: 
%   data: array of geo data as load in by loadfile.m (3 x OBS)
%   valueSelection: (optional parameter) method used to set the 
%                   geo position value if dates are repeated
%       1 = Average: values on the same day are averaged (default)
%       2 = First: uses the earliest value in the datset
%       3 = Last: uses the last value in the dataset
%       4 = Smallest error: chooses the value with the lowest error
%       5 = Largest error: chooses the value with the biggest error
%   errorSelection: (optional parameter) method used to set the
%                   error value if dates are repeated
%       1 = Average: values on the same day are averaged 
%       2 = First: uses the earliest value in the datset
%       3 = Last: uses the last value in the dataset
%       4 = Smallest error: chooses the value with the lowest error
%       5 = Largest error: (default)

