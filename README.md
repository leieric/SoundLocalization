This is Eric Lei (el536@cornell.edu) and Larry Tang's (lwt29@cornell.edu) final project for ECE 3140 (Embedded Systems). 

This software is written for the Freedom K64F board. We used two Adafruit MAX9814 microphones to localize the source of the sound. 
In brief, this code reads the ADC inputs into buffers, and correlates the received samples. The assumption is that both microphones should receive very similar copies of the same acoustic signal, just at different times. This result is the time delay between the two signals which is then used to find the incoming angle of the sound wave, which is then used to light up LED's that correspond to the direction of the waves. We used the CMSIS ARM DSP Library for many mathematical operations. 

Also in this repository are MATLAB scripts. The scipt 'angleSimulator.m' is what we used to do preliminary testing of our algorithm using simulated dummy data with noise added. The script 'script.m' is what we used to test the ADC read data from our K64F board while playing a 440 Hz sound from our phone speaker to see what the ADC data looked like. 

See https://drive.google.com/file/d/1k4c3TQOqHuEeU2OWg3cZb0oet9cfYI1h/view?usp=sharing for our final report on this project. 

If you want to run this code, you'll have to import this uVision project. See main.c for the majority of our algorithm. 
