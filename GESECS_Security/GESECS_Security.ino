#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <WiFiClientSecure.h>

//#include <TelegramCertificate.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#include <Wire.h>

#include <SoftwareSerial.h>

#define WIFI_SSID "GESECS Monitor"
#define WIFI_PASSWORD "darmayu123"

#define BOTtoken "5491660764:AAE25RzZeRoG-qRV5Ix9HScjYZH4xGUQiHA"
#define CHAT_ID "1079904158"

WiFiClientSecure client;

SoftwareSerial serial(D1, D2);

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
UniversalTelegramBot bot(BOTtoken, client);

int inputPin = D0;                                         // Memilih input PIR Sensor
int val = 0;                                               // Variable pembaca status pin
int pinSpeaker = D5;                               // Memilih pin buzzer, dengan pin PWM

String data;
char c;

void setup()
{
  Serial.begin(9600);
  serial.begin(9600);
  
  wifiConnecting();
  //  thing.add_wifi(WIFI_SSID, WIFI_PASSWORD);
  client.setInsecure();

  bot.sendMessage(CHAT_ID, "GESECS started up", "");

  pinMode(inputPin, INPUT);               // jadikan Sensor sebagai Input
  pinMode(pinSpeaker, OUTPUT);      // jadikan buzzer sebagai output
}

void loop()
{
  val = digitalRead(inputPin);

  while (serial.available() > 0) {
    delay(10);
    c = serial.read();
    data += c;
  }

  if (data.length() > 0) {
    Serial.print("Key Status : ");
    Serial.println(data);
    //    handleData();

    readPirData();

    data = "";
  }

}

void readPirData()
{
  if (data.toInt() == 0) {

    if (val == HIGH) {
      playTone(300, 160);

      delay(300);

      Serial.println("Terdeteksi suatu pergerakan!");

      Serial.println("=================================================================");
      Serial.println("Sending message to Telegram");
      Serial.println("=================================================================");

      bot.sendMessage(CHAT_ID, "Stranger detected!!", "");

//      if (bot.sendMessage(CHAT_ID, "Stranger detected!!", "")) {
//        Serial.println("TELEGRAM Successfully sent");
//      } else {
//        Serial.println("TELEGRAM NOT Successfully sent");
//      }

    } else {

      digitalWrite(pinSpeaker, HIGH);
      Serial.println("Motion ended!");

      delay(300);

    }

  } else {

    digitalWrite(pinSpeaker, HIGH);
    Serial.println("Safe");

    delay(300);

  }
}

void playTone(long duration, int freq)
{
  duration *= 1000;
  int period = (1.0 / freq) * 1000000;
  long elapsed_time = 0;
  while (elapsed_time < duration) {
    digitalWrite(pinSpeaker, HIGH);
    delayMicroseconds(period / 2);
    digitalWrite(pinSpeaker, LOW);
    delayMicroseconds(period / 2);
    elapsed_time += (period);
  }
}

void wifiConnecting()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);


  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
    serial.print(0);
  }

  Serial.println();
  Serial.print("Connected with SSID: ");
  Serial.println(WIFI_SSID);
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}
