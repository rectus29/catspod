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
const int wifiLEDPin =  9;
const int powerLEDPin =  10;
const int mqttLEDPin =  11;
const char potPIN =  A0;   
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char mqtt_server[] = "192.168.29.250";
const char subscribeTopic[] = "cmnd/catfeeder/POWER";
const char publishTopic[] = "stat/catfeeder/POWER";

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
  digitalWrite(wifiLEDPin, HIGH);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_mqtt(){
  mqtt.setServer(mqtt_server, 1883);
  mqtt.setCallback(callback);//Déclaration de la fonction de souscription
  reconnect();
}

void reconnect() {
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection .... ");
    if (mqtt.connect(DEVICEID, MQTTUSER, MQTTPASS)) {
      Serial.println(" CONNECTED");
      digitalWrite(mqttLEDPin, HIGH);
    }else{
      Serial.print(" FAILED to connect to MQTT broker, rc=");
      Serial.print(mqtt.state());
      Serial.println("try again in 5 second");
      digitalWrite(mqttLEDPin, LOW);
      delay(5000);
    }
  }
  mqtt.subscribe(subscribeTopic);
}

void runMotor(){
  int sensorValue = analogRead(potPIN);
  int timeToRun = 1000;//sensorValue * 10;

  digitalWrite(powerLEDPin, LOW);
  Serial.print("motorRun - ");
  Serial.print(timeToRun);
  Serial.println("ms");
  digitalWrite(relayPin, LOW);
  delay(timeToRun);
  digitalWrite(relayPin, HIGH);
  digitalWrite(powerLEDPin, HIGH);
}

void setup() {
  //init serial connection
  Serial.begin(9600);
  delay(3000); //waiting serial connection 
  Serial.println("---------------");
  Serial.println(" Cat Feeder V2 ");
  Serial.println("---------------");
  //init pin mode
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(powerLEDPin, OUTPUT);
  pinMode(mqttLEDPin, OUTPUT);
  pinMode(wifiLEDPin, OUTPUT);

  digitalWrite(powerLEDPin, HIGH);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);  
  setup_wifi();
  setup_mqtt(); 
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("- mqtt msg - ");
  Serial.print(topic);
  Serial.println(" -");
  //if(topic == "cmnd/catfeeder/POWER" /*&& (char) payload == 'ON'*/){
    runMotor();
  //}
}

void loop() {
  if (!mqtt.connected()) {
    reconnect();
  }
  mqtt.loop();
  //mqtt.subscribe()
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (digitalRead(buttonPin) == LOW) {
    runMotor();
  }
  //test here
  //Serial.println(digitalRead(buttonPin));
  //mqtt.publish(publishTopic, payload);
  //Serial.print("potar ");
  //Serial.println(analogRead(potPIN));
  delay(500);
}
