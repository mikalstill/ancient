function [energy, edb] = econtour(filename)

framesize = 512;
numsamples = wavread(filename, 'size');

% Read the whole file into one big vector
% Check the wavread doco for a better way of doing this
[samples, fsample, nbits] = wavread(filename, numsamples(1));

% Compute the energy contour
fcount = 1;
while fcount < numsamples(1) / framesize
    temp = samples((fcount - 1) * framesize + 1: fcount * framesize);
    
    % Calculate the energy, and the reference if required. Apply that reference
    energy(fcount) = temp' * temp;
    edb(fcount) = 10 * log10(energy(fcount));
    if(fcount == 1)
        eref = edb(1);
    end
    edb(fcount) = edb(fcount) - eref;
    
    % Next frame
    fcount = fcount + 1;
end