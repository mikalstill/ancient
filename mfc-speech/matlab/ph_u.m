function [ sdata, fsample ] = ph_u( filename )

% The framesize is 512
framesize = 512;

% Read in the wav file again
numsamples = wavread(filename, 'size');
[samples, fsample, nbits] = wavread(filename, numsamples(1));

% I have imperically determined that the following frames are used for /u/
start = 6 * framesize;
finish = (12 * framesize) - 1;

% This can be verified by plotting the /u/ sound. The hints for the assignment say that for /u/
% we should have periodicity of about 100Hz -- therefore I generate a graph with a period of 100Hz
%plot(samples(start:finish))
%grid on

% Therefore, a frame in the center is 8 * 512 to 9 * 512
sdata = samples(start:finish);