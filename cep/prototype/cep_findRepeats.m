function [found,list] = cep_findRepeats(dateList)
%
% function [found,list] = cep_findRepeats(dateList)
%
% Searches a vector of date values and finds any repeated dates
% Dates must be in asending order
%
% Imports:
%   dateList: vector of dates, usually data(1,:).
% Exports:
%   found: The number of repeated values found
%   list: list of the array indices of the repeated values

found = 0;
for i=1:length(dateList)-1
    if ((dateList(i)-dateList(i+1)) >= 0);
        found = found + 1;
        list(found) = i;
    end
end

if (found == 0)
    list(1) = 0;
    list(1) = [];
end
