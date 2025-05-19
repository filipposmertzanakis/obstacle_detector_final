#include <WiFi.h>
#include <HTTPClient.h>
#include "ThingSpeak.h"  

#define TRIG_PIN 5
#define ECHO_PIN 8

const char* ssid = "VODAFONE_1974";
const char* password = "123456789";
const char* apiKey = "BDG0IJORP5XSOGCT";  
const char* server = "http://api.thingspeak.com/update";

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

void loop() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float distance = duration * 0.0343 / 2;

  if (duration > 0 && distance < 400) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = String(server) + "?api_key=" + apiKey + "&field1=" + String(distance);

      http.begin(url);
      int httpCode = http.GET();
      if (httpCode > 0) {
        Serial.println("Data sent to ThingSpeak");
      } else {
        Serial.print("Failed to send. HTTP error: ");
        Serial.println(httpCode);
      }
      http.end();
    } else {
      Serial.println("WiFi disconnected!");
    }
  } else {
    Serial.println("No echo received or out of range.");
  }

  delay(15000); 
}
