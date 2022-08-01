#include <ThingerESP8266.h>

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <WiFiClientSecure.h>

#include <Wire.h>

#include <Adafruit_INA219.h>

#include <SoftwareSerial.h>

#define WIFI_SSID "GESECS Monitor"
#define WIFI_PASSWORD "darmayu123"

#define USERNAME "Kuplay"
#define DEVICE_ID "Gesecs_ID"   //Your Device Name
#define DEVICE_CREDENTIAL "F_qxLGG0afC6er%z"

#define SENSOR D5

WiFiClientSecure client;

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

SoftwareSerial serial(D6, D7);

Adafruit_INA219 ina219;                      // Memilih pin buzzer, dengan pin PWM

String data;
char c;

long currentMillis = 0;
long previousMillis = 0;
long nowMillis = 0;
long pastMillis = 0;
int interval = 1000;

float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void setup()
{
  Serial.begin(9600);
  serial.begin(9600);

  while (!Serial) {
    // will pause Zero, Leonardo, etc until serial console opens
    delay(1);
  }

  wifiConnecting();
  //  thing.add_wifi(WIFI_SSID, WIFI_PASSWORD);
  //  client.setInsecure();

  pinMode(SENSOR, INPUT_PULLUP);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);

  uint32_t currentFrequency;
  ina219.begin();

  postData();
}

void loop()
{

  readWaterFlowData();

  readElectricData();

  showData();


  Serial.println("=================================================================");
  Serial.println("Send data to Thinger.IO");
  Serial.println("=================================================================");

  thing.handle();

  delay(500);
}

void showData()
{
  serial.print(loadvoltage);
  serial.print(",");
  serial.print(current_mA);
  serial.print(",");
  serial.print(power_mW);
  serial.print(",");
  serial.print(flowRate);

  Serial.println("=================================================================");
  Serial.println("Sending message to Serial");
  Serial.println("=================================================================");
}

void postData()
{
  thing["data"] >> [](pson & out) {
    out["Voltage"] = loadvoltage;
    out["Current"] = current_mA;
    out["Power"] = power_mW;
    out["Flow"] = flowRate;
  };
}

void readElectricData()
{
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  Serial.print("Bus Voltage   : "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage : "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage  : "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current       : "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power         : "); Serial.print(power_mW); Serial.println(" mW");
}

void readWaterFlowData()
{
  currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    pulse1Sec = pulseCount;
    pulseCount = 0;
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;

    Serial.println("");
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.println("L/min");
    //    Serial.print("\t");       // Print tab space
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(totalMilliLitres / 1000);
    Serial.println("L");
    Serial.println("");
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
