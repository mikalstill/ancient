function [ sdata, fsample ] = ph_sh( filename )

% The framesize is 512
framesize = 512;

% Read in the wav file again
numsamples = wavread(filename, 'size');
[samples, fsample, nbits] = wavread(filename, numsamples(1));

% I have imperically determined that the following frames are used for /sh/
start = 70 * framesize;
finish = (74 * framesize) - 1;

% This can be verified by plotting the /sh/ sound. The hints for the assignment say that for /sh/
% the plot exhibits non-periodicity and a high-frequency "ragged" waveform
%plot(samples(start:finish))
%grid on

% Therefore, a frame in the center is 8 * 512 to 9 * 512
sdata = samples(start:finish);