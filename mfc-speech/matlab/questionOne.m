function [] = questionOne(filename)

% Generate the energy contour (part b)
[en, edb] = econtour(filename);

% Generate the autocorrelation (part c)
[ac, ff] = autocorr(filename, en);




% Plot all of these
subplot(3, 1, 1), plot(edb)
grid on
ylabel('Energy (dB)');
title('Energy, with first frame as reference');

subplot(3, 1, 2), plot(ac)
grid on
title('Normalized autocorrelation');

subplot(3, 1, 3), plot(ff)
grid on
title('Fundamental frequency');