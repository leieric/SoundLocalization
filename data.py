import serial

serial = serial.Serial(port='COM5', baudrate=115200)
f = open('data.txt', 'w')
i = 0
while i < 5000:
	data = serial.read()
	f.write(data)
	i = i+1

f.close();
