import sys
print (sys.version)
import serial
serialConnection = serial.Serial()
serialConnection.baudrate = 9600
serialConnection.port = 9
print(serialConnection)

def parseLine(line):
	splitLine = line.split(" ")
	pan = (180/1024) * int(splitLine[0][2:])
	tilt = (180/1024) * int(splitLine[1][2:])
	x = int(splitLine[2][2:])
	p1 = 1.17e-13
	p2 = -2.3335e-10
	p3 = 1.9111e-07
	p4 = -8.3856e-05
	p5 = 0.021551
	p6 = -3.3141
	p7 = 292.46
	distance =  p1*x**6 + p2*x**5 + p3*x**4 + p4*x**3 + p5*x**2 + p6*x +p7 
	dataPoint = str(pan) + "," + str(tilt) + "," + str(distance) + "\n"
	return dataPoint

serialConnection.open()
maxDataPoints = 5000
with open('data4.csv','w') as datafile:
	for m in range(maxDataPoints):
		line = serialConnection.readline()
		line = line.decode(encoding='UTF-8',errors='strict').strip()
		parse = parseLine(line)
		datafile.write(parse)
		print(str(m) + "/" + str(maxDataPoints))
		print(line)
		print(parse)
	serialConnection.close()

