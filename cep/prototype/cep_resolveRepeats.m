function [data] = cep_resolveRepeats(data, valueSelection, errorSelection)
%
% function [newData] = cep_resolveRepeats(data)
% function [newData] = cep_resolveRepeats(data, valueSelection, errorSelection)
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

% setup default value selection
if nargin < 3; errorSelection = 5;end;
if nargin < 2; valueSelection = 1;end;

% Setup constants
AVERAGE = 1;
FIRST = 2;
LAST = 3;
SMALLEST_ERROR = 4;
LARGEST_ERROR = 5;


% find repeated dates
[numFound,list] = cep_findRepeats(data(1,:));

tempList = list;

for i=1:numFound
    list(i) = tempList(numFound + 1 - i);
end

% if any repeats are found, resolve them by the chosen method
while (length(list) > 0)
    numRepeats = 1;
    
    while ((length(list) > 1) & list(1) == list(2))
        check = 1
        numRepeats = numRepeats + 1;
        list(1) = []; % remove element from list
    end
     
   % --- value selection methods ---
    if (valueSelection == AVERAGE)
        sum = 0;
        for j = 0:numRepeats
            sum = sum + data(2,list(1)+j);
        end
        value = sum/numRepeats+1;
    end
    if (valueSelection == FIRST)
        value = data(2,list(1));
    end
    if (valueSelection == LAST)
        value = data(2,list(1)+numRepeats);
    end
    if (valueSelection == SMALLEST_ERROR)
        error = data(3,list(1));
        value = data(2,list(1));
        for j = 1:numRepeats
            if (data(3,list(1)+j) < error)
                error = data(3,list(1)+j);
                value = data(2,list(1)+j);
            end
        end
    end
    if (valueSelection == LARGEST_ERROR)
        error = data(3,list(1));
        value = data(2,list(1));
        for j = 1:numRepeats
            if (data(3,list(1)+j) > error)
                error = data(3,list(1)+j);
                value = data(2,list(1)+j);
            end
        end
    end
    
   % --- error selection methods ---
    if (errorSelection == AVERAGE)
        sum = 0;
        for j = 0:numRepeats
            sum = sum + data(3,list(1)+j);
        end
        error = sum/numRepeats;
    end
    if (errorSelection == FIRST)
        error = data(3,list(1));
    end
    if (errorSelection == LAST)
        error = data(3,list(1)+numRepeats);
    end
    if (errorSelection == SMALLEST_ERROR)
        error = data(3,list(1));
        for j = 1:numRepeats
            if (data(3,list(1)+j) < error)
                error = data(3,list(1)+j);
            end
        end
    end
    if (errorSelection == LARGEST_ERROR)
        error = data(3,list(1));
        value = data(2,list(1));
        for j = 1:numRepeats
            if (data(3,list(1)+j) > error)
                error = data(3,list(1)+j);
            end
        end
    end
    
   % --- replace repeat date values ---
   % delete excess array elements
   for i=1:numRepeats
       data(:,list(1)+1) = [];
   end
   % fill remaining element with values
   data(2,list(1)) = value;
   data(3,list(1)) = error;
   
   % Delete fixed item from list of errors
   list(1) = [];
end



