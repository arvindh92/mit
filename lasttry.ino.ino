

/*
 Basic MQTT example

 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>




// Update these with values suitable for your network.
byte mac[]    = { 0x98, 0x4F, 0xEE, 0x05, 0x3F, 0x1C };
//IPAddress ip(172, 16, 0, 100);
//IPAddress server(172, 16, 0, 2);
const char host[] = "quickstart.messaging.internetofthings.ibmcloud.com";

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("d:quickstart:iotsample-arduino:984F053F1C")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
     client.publish("iot-2/evt/status/fmt/json","{'temp':'30'}");
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
     // Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(9600);

  client.setServer(host, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  //client.loop();
  delay(2000);
  String str1 = "{\"sensor\":\"Temperature\",\"val\":"+Serial.readString()+"}";
  
  char charBuf[100];
  str1.toCharArray(charBuf,100);
   client.publish("iot-2/evt/status/fmt/json",charBuf);
}
