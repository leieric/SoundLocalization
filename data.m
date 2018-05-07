DATA = importdata('dataClap.txt');
dat1 = DATA(:, 1);
dat2 = DATA(:, 2);

t = 1:length(dat1);

clf, close all;
figure(1)
plot(t, dat1), hold on
plot(t, dat2)

figure(2)
Fs = 120;
N = length(dat1);
F0 = Fs/N;
f = [-N/2*F0:F0:(N/2-1)*F0];
plot(f, abs(fftshift(fft(dat1))/N)), hold on
plot(f, abs(fftshift(fft(dat2))/N))