function [found] = cep_findRepeats(data1)

truefalse = 0
for i=1:length(data1)-1
    if ((data1(i)-data1(i+1)) >= 0);
        truefalse = 1;
        i
    end
end