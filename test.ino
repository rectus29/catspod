#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <WiFiNINA.h>

#include "secrets.h"

void setup_wifi();
void reconnect();

static char payload[256];


#define TOKEN ""
#define DEVICEID "Cat_feeder"


const int buttonPin = 7;     
const int relayPin =  8;     
const char potPIN =  A0;   
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char mqtt_server[] = "192.168.29.250";
const char publishTopic[] = "v1/devices/me/telemetry";

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection .... ");
    if (mqtt.connect(DEVICEID, MQTTUSER, MQTTPASS)) {
      Serial.print(" CONNECTED");
      digitalWrite(LED_BUILTIN, HIGH);
    }else{
      Serial.print(" FAILED to connect to MQTT broker, rc=");
      Serial.print(mqtt.state());
      Serial.println("try again in 5 second");
      digitalWrite(LED_BUILTIN, LOW);
      delay(5000);
    }
  }
}

void runMotor(int timeToRun){
  digitalWrite(2, HIGH);
  Serial.print("motorRun - ");
  Serial.print(timeToRun);
  Serial.println("ms");
  digitalWrite(relayPin, LOW);
  delay(timeToRun);
  digitalWrite(relayPin, HIGH);
  digitalWrite(2, LOW);
}


void setup() {
  //init serial connection
  Serial.begin(9600);
  delay(3000); //waiting serial connection 
  Serial.println("---------------");
  Serial.println(" Cat Feeder V2 ");
  Serial.println("---------------");
  setup_wifi();
  mqtt.setServer(mqtt_server, 1883);
  //init pin mode
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);   
}


void loop() {
  if (!mqtt.connected()) {
    reconnect();
  }
  mqtt.loop();
  //mqtt.subscribe()
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (digitalRead(buttonPin) == LOW) {
    int sensorValue = analogRead(potPIN);
    int timeToRun = sensorValue * 10;
    runMotor(timeToRun);
  }


  //test here
  Serial.println(digitalRead(buttonPin));
  //mqtt.publish(publishTopic, payload);
  Serial.print("potar ");
  Serial.println(analogRead(potPIN));
  delay(1000);
}
