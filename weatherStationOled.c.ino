#include <Arduino.h>
#include "SSD1306Wire.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

SSD1306Wire display(0x3c, D3, D4); // I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN

StaticJsonDocument<800> doc;
  
void setup() {

  Serial.begin(115200);

  display.init();
  display.setContrast(255);
  display.clear();

  WiFi.begin("Wifi", "redhouse21");
  display.drawString(0, 10, "Connecting...");
  display.display();
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(800);
  }
}

void loop() {
  display.clear();

  WiFiClient client;
  HTTPClient http;
  http.begin(client, "http://api.openweathermap.org/data/2.5/weather?q=Dallas,USA&APPID=8bc6d7b5ee9c41a095d50aed9f2d8e89");
  int httpCode = http.GET();

  String response = http.getString();

  deserializeJson(doc, response); // parse the json response
  int temp = doc["main"]["temp"];
  temp = temp - 273.15;
  int humid = doc["main"]["humidity"];
  const char* name = doc["name"];
      
  display.clear();
  display.drawString(0, 0, name);
  display.drawString(0, 10, "Temperature: " + String(temp));
  display.drawString(0, 20, "Humidity: " + String(humid));

  display.display();
  
  http.end(); //close
  
  delay(10000);
}
