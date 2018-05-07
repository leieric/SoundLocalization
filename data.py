import serial

serial = serial.Serial(port='COM5', baudrate=115200)
f0 = open('data.txt', 'w')
# f1 = open('data1.txt', 'w')
i = 0
while i < 30000:
	data = serial.read()
	f0.write(data)
	# data = serial.read()
	# f1.write(data)
	i = i+1

f0.close()
# f1.close()
