import serial
import time
import paho.mqtt.client as mqtt

def on_publish():
    print ("Temperatura inviata")

serialPort = serial.Serial(port="COM4", baudrate=9600, bytesize=8)
broker_address = "mqtt.ssh.edu.it"
client = mqtt.Client("Reggiani")
client.connect(broker_address)


while True:
   try:
       string = serialPort.readline()
       time.sleep(5)
       client.publish("5h/reggiani/temp", string.decode())
       print(string.decode())
       client.on_publish = on_publish()
   except KeyboardInterrupt:
       print ("<---connection interrupted--->")



serialPort.close()

