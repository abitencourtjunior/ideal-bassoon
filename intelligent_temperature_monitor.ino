#include <Wire.h>
#include "SSD1306Wire.h"
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

DynamicJsonDocument doc(4096);
SSD1306Wire  display(0x3c, D1, D2);

#define URL_SERVER "http://goweather.herokuapp.com/weather/Jaguaruna"

void setup()
{
  WiFiManager wifiManager;
  wifiManager.autoConnect("Monitor de Informacoes");
  wifiManager.setConfigPortalTimeout(180);
  Serial.begin(115200);
  display.init();
  display.flipScreenVertically();
}
void initScreen()
{
  for (int counter = 0; counter <= 100; counter++)
  {
    display.clear();
    display.drawProgressBar(0, 32, 120, 10, counter);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawString(63, 10, "Iniciando");
    display.display();
    delay(10);
  }
}

void get_weather_by_days()
{
  WiFiClient clientWifi;
  HTTPClient http;
  http.begin(clientWifi, URL_SERVER);
  Serial.printf("[HTTP] Server...: %s\n", URL_SERVER);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.GET();

  if (httpCode > 0)
  {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
    {
      DeserializationError error = deserializeJson(doc, http.getString());

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      
    const char* temperature = doc["temperature"];
    Serial.println(temperature);
      
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}



void loop()
{
  initScreen();
  while(true){
    get_weather_by_days();
    delay(15000);
  }
}
