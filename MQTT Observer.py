import paho.mqtt.client as mqtt

def on_message(client, userdata, msg):
    print('msg: {} from topic: {}'.format(msg.payload.decode(), msg.topic))

def on_connect(client, userdata, flags, rc):
    print('Connection with code: {}'.format(mqtt.connack_string(rc)))
    client.subscribe('5h/#', qos=0)


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
