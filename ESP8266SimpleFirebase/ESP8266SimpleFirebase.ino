#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "esp8266projects-d18be-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "EYW3KibC6x8KdrVWOaiUQ9pivOoigxkv7pUHYylb"
#define WIFI_SSID "Dafid@INETMEDIA"
#define WIFI_PASSWORD "passwordeopo"

#include "DHT.h"
#define DHTPIN D4
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

int lampStatus;
int ledPin = D2; 

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(ledPin, OUTPUT);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {

  //delay(2000);
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  Firebase.setFloat("humidity", h);
  if (Firebase.failed()) {
      Serial.print("setting /humidty failed:");
      Serial.println(Firebase.error());  
      return;
  }
  
  Firebase.setFloat("temperature", t);
  if (Firebase.failed()) {
      Serial.print("setting /temperature failed:");
      Serial.println(Firebase.error());  
      return;
  }

  lampStatus = Firebase.getInt("lampStatus");

  if(lampStatus == 1){
      digitalWrite(ledPin, HIGH);
   } else {
      digitalWrite(ledPin, LOW);   
   }

  Serial.print("Humidity : ");
  Serial.println(h);
  Serial.print("Temperature : ");
  Serial.println(t);
  Serial.print("Lamp Status : ");
  Serial.println(lampStatus);
  Serial.println("===========================");
}
