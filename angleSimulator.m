F = 2000; %frequency of sound wave [Hz]
T = 1/F; %period [sec]
Fs = 400000; %sampling rate [samples/sec]
Ts = 1/Fs; %sampling period

angle = 43; % angle of incoming waves [degrees]
theta = (pi/180)*(angle);
v0 = 343; % speed of sound waves [m/s]
d0 = 0.2; %distance between the two microphones[m]
t0 = d0*sin(theta)/v0 % calculated time delay between the signals [sec]

tRecord = 0.01; %Length of recording [sec]

tt = (0:Ts:tRecord); %record for specified length

%signal received by first microphone
x1 = cos(2*pi*F*tt) + cos(2*pi*0.2*F*tt) + 2*cos(2*pi*1.5*F*tt);
%time delayed signal received by second microphone
x2 = exp(-10*tt).*cos(2*pi*F*(tt-t0)) + exp(-5*tt).*cos(2*pi*0.2*F*(tt-t0)) + 2*exp(-12*tt).*cos(2*pi*1.501*F*(tt-t0)); 

%init lowpass filter
d = designfilt('lowpassiir','FilterOrder',8, ...
     'PassbandFrequency',15e3,'PassbandRipple',0.2, ...
     'SampleRate',Fs);
% d = designfilt('lowpassfir','PassbandFrequency',0.25, ...
%          'StopbandFrequency',0.35,'PassbandRipple',0.5, ...
%          'StopbandAttenuation',150,'DesignMethod','kaiserwin');

close all
hold on
plot(tt, x1);
plot(tt, x2);
title('Signals w/ no noise');

cx = 0.001*xcorr(x2, x1);
tc = (0:Ts:(2*tRecord)); %correlation of length N signals is length 2N-1
figure
plot(tc, cx);
title('Correlation w/ no noise');

y1 = awgn(x1, -5, 'measured'); %add noise to signal 1
y2 = awgn(x2, -5, 'measured'); %add noise to signal 2

% filter the noisy signals
y1 = filter(d, y1);
y2 = filter(d, y2);

figure
hold on
plot(tt, y1);
plot(tt, y2);
title('Signals w/ noise');

cxn = 0.001*xcorr(y2, y1);
figure
plot(tc, cxn);
title('Correlation w/ noise');

%find the time delay of the signals using correlation
deltat = tc(find(cx==max(cx))) - tRecord
deltatn = tc(find(cxn==max(cxn))) - tRecord

%predict incoming angle using calculated time delay
thetaPredict = 180/pi*asin(deltat*v0/d0)
thetaPredictNoise = 180/pi*asin(deltatn*v0/d0)
% NOTE: make sure that argument to inverse sin is not greater than 1


