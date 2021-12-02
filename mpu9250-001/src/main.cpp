#include <Arduino.h>
#include "MPU9250.h"
#include <ArduinoOSCWiFi.h>
#include "SSD1306Wire.h"

const char *ssid = "test001";
const char *pwd = "worktest";
// const IPAddress ip(192, 168, 0, 201);
// const IPAddress gateway(192, 168, 0, 1);
// const IPAddress subnet(255, 255, 255, 0);
const char *host = "192.168.0.140"; // destino
const int publish_port = 1234;

#define I2C_SDA 5 // 22
#define I2C_SCL 4 // 21

MPU9250 mpu;
SSD1306Wire display(0x3c, I2C_SDA, I2C_SCL);
String rpy;
const String ip;
float roll, pitch, yaw;



void print_roll_pitch_yaw()
{
  Serial.print("Yaw, Pitch, Roll, Temp: ");
  Serial.print(mpu.getYaw(), 2);
  Serial.print(", ");
  Serial.print(mpu.getPitch(), 2);
  Serial.print(", ");
  Serial.println(mpu.getRoll(), 2);
  Serial.print(", ");
  Serial.println(mpu.getTemperature(), 2);
}

void send_osc()
{
  roll = mpu.getRoll();
  pitch = mpu.getPitch();
  yaw = mpu.getYaw();
  rpy = "r" + String(roll) + "," + "p" + String(pitch) + "," + "y" + String(yaw);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Wire.begin(I2C_SDA, I2C_SCL);

  display.init();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.clear();
  display.drawString(0, 0, "ON");
  display.display();

  WiFi.disconnect(true, true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pwd);
  // WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    display.clear();
    display.drawString(0, 0, "connecting to test001");
    display.display();
    delay(500);
  }
  Serial.print("WiFi connected, IP = ");
  Serial.println(WiFi.localIP());
  OscWiFi.publish(host, publish_port, "/v", rpy)
      ->setFrameRate(60.f);

  if (!mpu.setup(0x68))
  {
    while (1)
    {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
        display.clear();
        display.drawString(0, 0, "no mpu");
        display.display();
        delay(5000);
    }
  }
}

void loop()
{

  if (mpu.update())
  {
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 25)
    {
      print_roll_pitch_yaw();
      OscWiFi.update();
      send_osc();
      display.clear();
      display.drawString(0, 0, ip);
      display.drawString(0, 8, rpy);
      display.display();
      prev_ms = millis();
    }
  }
}