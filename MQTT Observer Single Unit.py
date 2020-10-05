import serial
import time
import paho.mqtt.client as mqtt


def on_message(client, userdata, msg):
    serialPort = serial.Serial(port="COM6", baudrate=9600, bytesize=8)
    T_MIN = 26.0
    T_MAX = 29.4
    print('msg: {} from topic: {}'.format(msg.payload.decode(), msg.topic))
    temperature = float(msg.payload[:4].decode())
    if temperature < T_MIN:

        serialPort.write(b'B') #dropping tempertature
    elif temperature > T_MAX:
        serialPort.write(b'A') #overheating
    else:
        serialPort.write(b'C') #control




def on_connect(client, userdata, flags, rc):
    print('Connection with code: {}'.format(mqtt.connack_string(rc)))
    client.subscribe('5h/pietri/temp', qos=0)


def on_subscribe(client, userdata, mid, granted_qos):
    print('subscribed topic with QoS: {}'.format(granted_qos[0]))


def main():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_subscribe = on_subscribe
    client.on_message = on_message
    client.connect(host='mqtt.ssh.edu.it')

    try:
        client.loop_forever()
    except KeyboardInterrupt:
        print("<--- Disconnected --->")


if __name__ == '__main__':
    main()
