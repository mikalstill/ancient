function [ ] = questionTwo( filename )

% The framesize is 512
framesize = 512;

% Find the /u/ frame and perform a frequency domain analysis
[sdata, fsample] = ph_u(filename);
[lms, cep, smoothed] = fd_analyse(sdata, fsample, framesize);

% Plot all of this
figure(1);
subplot(3, 1, 1), plot(lms);
grid on
title('Log magnitude spectrum of /u/');

subplot(3, 1, 2), plot(cep);
grid on
title('Cepstrum of /u/');

subplot(3, 1, 3), plot(smoothed);
grid on
title('Smoothed cepstrum of /u/');


% Find the /sh/ frame and perform a frequency domain analysis
[sdata, fsample] = ph_sh(filename);
[lms, cep, smoothed] = fd_analyse(sdata, fsample, framesize);

% Plot all of this
figure(2);
subplot(3, 1, 1), plot(lms);
grid on
title('Log magnitude spectrum of /sh/');

subplot(3, 1, 2), plot(cep);
grid on
title('Cepstrum of /sh/');

subplot(3, 1, 3), plot(smoothed);
grid on
title('Smoothed cepstrum of /sh/');