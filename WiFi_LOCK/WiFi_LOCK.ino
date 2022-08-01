#include <SoftwareSerial.h>

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#define WIFI_SSID "WiFi_KEY"
#define WIFI_PASSWORD "WiFi_KEY_XXX"

SoftwareSerial serial(D1, D2);

void setup() {
  Serial.begin(9600);
  serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

}

void loop() {
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
    serial.print(0);
  }

  serial.print(1);

  Serial.println("Connecting Success !!!");
  delay(100);
}
