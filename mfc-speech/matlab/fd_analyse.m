function [ lms, cep, smoothed ] = fd_analyse( sdata, fsample, framesize )

% Generate a hamming window and then apply that window
hwind = hamming(framesize);
fcount = 1;
while fcount < framesize
    hammed(fcount) = sdata(fcount) * hwind(fcount);
    
    % Next frame
    fcount = fcount + 1;
end

% Perform a FFT
fdata = abs(fft(hammed));

% Square each value and then perform a log
fcount = 1;
while fcount < length(fdata)
    lms(fcount) = 20 * log10(fdata(fcount));
    
    % Next frame
    fcount = fcount + 1;
end

cep = real(ifft(lms));

% Now we need the smoothed cepstrum
tcut = fsample / 400;
scep = cep;
for i = tcut:(framesize + 2 - tcut)
    scep(i) = 0;
end

smoothed = real(fft(scep));