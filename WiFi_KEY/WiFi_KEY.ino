#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#define WIFI_SSID "WiFi_KEY"
#define WIFI_PASSWORD "WiFi_KEY_XXX"

void setup() {
  Serial.begin(9600);

  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Access Point \"");
  Serial.print(WIFI_SSID);
  Serial.println("\" started");
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(".");

}
