#include <Arduino.h>
#include <WiFiS3.h>
#include <ArduinoBLE.h>

#include "WIFIPASS.h"
#include "BLUETOOTH.h"

const int trigPin = 9;
const int echoPin = 10;
const int buzzPin = 4;
const int relayPin = 2;

const int yellowLedPin = 12;
const int redLedPin = 7;

const float SPEED_OF_SOUND = 0.0343;

unsigned long timepassed;
unsigned long triggertime = 0;

float duration, distance;

int status = WL_IDLE_STATUS;

BLEService fileTransferService(dataCharacteristicsUUID);
BLECharacteristic dataCharacteristic(dataCharacteristicsUUID, BLEWrite | BLERead, "");
WiFiServer server(80);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  Serial.begin(9600);

  //while(status != WL_CONNECTED){
    //Serial.print("Attempting to connect to SSID: ");
    //Serial.println(ssid);

    //status = WiFi.begin(ssid, pass);

    //delay(10000);
  //}
  //server.begin();

  PrintWifiStatus();
}

void loop() {
  
  WiFiClient client = server.available();

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*SPEED_OF_SOUND)/2;

  Serial.print("distance: ");
  Serial.println(distance);

  if (distance <= 50) {
    digitalWrite(yellowLedPin, HIGH);
    if (triggertime == 0){
      triggertime = millis();
    }
    if (millis() - triggertime > 4000){
        digitalWrite(redLedPin, HIGH);
        digitalWrite(relayPin, HIGH);
        //tone(buzzPin, 3500, 500);
    }
    else{
      digitalWrite(redLedPin, LOW);
      digitalWrite(relayPin, LOW);
    }
  }
  else{
    triggertime = 0;
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(redLedPin, LOW);
    digitalWrite(relayPin, LOW);
  }
}


void PrintWifiStatus(){
  Serial.print("IP Adress: ");
  Serial.println(WiFi.localIP());

  Serial.print("Signal strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}