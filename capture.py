import serial

ser = serial.Serial("com3", baudrate=1000000, timeout=0.1)

bytes = b''
for i in range(20000):
    bytes += ser.read(100)

print(bytes.decode("utf-8").replace("\n", ""))