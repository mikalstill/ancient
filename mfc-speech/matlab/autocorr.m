function [acorr, ffreq] = autocorr(filename, energy)

framesize = 512;
numsamples = wavread(filename, 'size');
vthresh = 0.375;

% Read the whole file into one big vector
% Check the wavread doco for a better way of doing this
[samples, fsample, nbits] = wavread(filename, numsamples(1));

% We need to know kmin and kmax before we can continue
kmin = fsample / 200;
kmax = fsample / 80;

% Compute the autocorrelation
fcount = 1;
while fcount < numsamples(1) / framesize
    temp = samples((fcount - 1) * framesize + 1: fcount * framesize);
    
    for count = kmin:kmax
        acalc(count) = temp(1:framesize - count)' * temp(1 + count: framesize);
    end
    
    % Now normalize
    [amax, indexmax] = max(acalc);
    acorr(fcount) = amax / energy(fcount);
    
    if (acorr(fcount) > vthresh)
        ffreq(fcount) = fsample / indexmax;
    end 
    
    % Next frame
    fcount = fcount + 1;
end