import serial
import time
import requests

serialPort = serial.Serial(port="COM4", baudrate=9600, bytesize=8)
apyKey = "IGL93FNPR52TAI0L"
field = "field1"
baseUrl = "http://api.thingspeak.com/update?api_key=" + apyKey + "&" + field + "="

while True:
    try:

        temperature = []

        for i in range(4):
            string = float(serialPort.readline().decode())
            temperature[i]=string
            time.sleep(5)

        media = sum(string)
        r = requests.get(baseUrl + str(media/4))
        print(baseUrl + str(media/4))
        print("n:{} retStatus:{} retText:{}".format(str(media/4), r.status_code, r.text))

    except KeyboardInterrupt:
        print("<---connection interrupted--->")
        break
serialPort.close()
