#include <SoftwareSerial.h>

#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_SDA D1
#define OLED_SCL D2
#define OLED_RST -1
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixel

SoftwareSerial serial(D5, D6);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String data;
char c;

void setup() {
  
  Serial.begin(9600);
  serial.begin(9600);

  oledSetup();

}

void loop() {

  getData();
  
  delay(500);
}

void getData()
{
  while (serial.available() > 0) {
    delay(10);
    c = serial.read();
    data += c;
  }

  if (data.length() > 0)
  {
    Serial.println(data);

    float v = getValue(data, ',', 0).toFloat();
    float mA = getValue(data, ',', 1).toFloat();
    float mW = getValue(data, ',', 2).toFloat();
    float flow = getValue(data, ',', 3).toFloat();

    showData(v, mA, mW, flow);

    data = "";
  }
}

void showData(float v, float mA, float mW, float flow)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("GESECS Display");
  display.setCursor(0, 10);
  display.print("Voltage : ");
  display.print(v);
  display.print(" V");
  display.setCursor(0, 20);
  display.print("Current : ");
  display.print(mA);
  display.print(" mA");
  display.setCursor(0, 30);
  display.print("Power   : ");
  display.print(mW);
  display.print(" mW");
  display.setCursor(0, 40);
  display.print("Flow    : ");
  display.print(flow);
  display.print(" L/min");
  display.display();
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void oledSetup()
{

  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("GESECS Actived");
  display.display();
}
