import serial

def getSerialConnection(padSideByteString):
	padSideStr = "0" if (padSideByteString == "left") else "1"

	s = serial.Serial("/dev/ttyACM0", 9600)
	s.setDTR(1)

	#Send 9: Gief pad side from ttyACM0
	s.write("9\r\n")
	padSide = s.readline()

	if padSide[0] != padSideStr:
		#Turns out he was the other side, so ttyACM1 has our pad! We connect to him now!
		s.close()

		s = serial.Serial("/dev/ttyACM1", 9600)
		s.setDTR(1)
	
	return s

s = getSerialConnection("left")
s.write("D\r\n")
s.close()
