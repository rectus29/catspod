/*
  The circuit:
  - Note: on most Arduinos there is already an LED on the board
    attached to 13
*/
/*
#include <Wire.h>
#include <DS3231.h>

const int buttonPin = 7;     
const int relayPin =  8;     
const int ledPin =  13;  
const char* potPIN =  "A0";      
struct Horaire {
  int hour;
  int min;
  int sec;
  };
RTClib rtcObject;
//init distribution time table
const Horaire distrib[4] = {{9,0,0}, {16,50,0}, {17,19,0}, {22,50,0}};




void setup() {
  Serial.begin(9600);
  Wire.begin();
  //init pin mode
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  pinMode(ledPin , OUTPUT);  
  pinMode(2 , OUTPUT);  
  pinMode(buttonPin, INPUT_PULLUP);    
  //for init only
  //initDate();
}

void loop() {
  //blink led
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(1000);              
  digitalWrite(LED_BUILTIN, LOW);
  //retreive date from RTC Module 
  DateTime dateTime = rtcObject.now();   
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (checkTimeTable(dateTime) || digitalRead(buttonPin) == LOW) {
    int sensorValue = analogRead(A0);
    int timeToRun = sensorValue * 10;
    runMotor(timeToRun);
  }
  serialPrintTime(dateTime);
  //delay(500);
}

bool checkTimeTable(DateTime time){
    for(int i =0; i < 4; i++){
      if( distrib[i].hour == time.hour() &&
          distrib[i].min == time.minute() &&
          distrib[i].sec == time.second()
          )
          return true;
    }
    return false;
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

void serialPrintTime(DateTime time){
  Serial.print("date : ");
  Serial.print(time.day());
  Serial.print("/");
  Serial.print(time.month());
  Serial.print("/");
  Serial.print(time.year());
  Serial.print("\t Heure : ");
  Serial.print(time.hour());
  Serial.print(":");
  Serial.print(time.minute());
  Serial.print(".");
  Serial.println(time.second());  
}

//use for init DS3231 only 
void initDate(){
  DS3231 Clock;
  Clock.setClockMode(false);  // false set to 24h, true to 12h
  Clock.setYear(2023);
  Clock.setMonth(3);
  Clock.setDate(12);
  Clock.setDoW(6);
  Clock.setHour(00);
  Clock.setMinute(00);
  Clock.setSecond(01);
}*/
