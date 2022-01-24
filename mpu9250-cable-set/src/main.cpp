#include <Arduino.h>
#include "MPU9250.h"

#define I2C_SDA 5 // 22
#define I2C_SCL 4 // 21

MPU9250 mpu;

String a = "0";
String b = "0";

void print_roll_pitch_yaw()
{
    Serial.println(a + "," + b + "," + "r" + String(mpu.getRoll()) + "," + "p" + String(mpu.getPitch()) + "," + "y" + String(mpu.getYaw()));
}

void print_calibration()
{
    Serial.println("< calibration parameters >");
    Serial.println("accel bias [g]: ");
    Serial.print(mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasY() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasZ() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.println();
    Serial.println("gyro bias [deg/s]: ");
    Serial.print(mpu.getGyroBiasX() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasY() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasZ() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.println();
    Serial.println("mag bias [mG]: ");
    Serial.print(mpu.getMagBiasX());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasY());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasZ());
    Serial.println();
    Serial.println("mag scale []: ");
    Serial.print(mpu.getMagScaleX());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleY());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleZ());
    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL);
    pinMode(0, INPUT_PULLUP);
    pinMode(2, INPUT_PULLUP);
    delay(2000);

    if (!mpu.setup(0x68))
    {
        while (1)
        {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }

    mpu.setAccBias(-253.92, -246.64, -187.51);
    mpu.setGyroBias(-1.13, 1.48, -2.81);
    mpu.setMagBias(105.40, 333.36, -266.90);
    mpu.setMagScale(2.24, 0.71, 0.88);
    mpu.setMagneticDeclination(-3.13);

    print_calibration();
    mpu.verbose(false);
}

void loop()
{

    if (mpu.update())
    {
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 25)
        {
            if (digitalRead(0))
            {
                a = "0";
            }
            else
            {
                a = "1";
            }

            if (digitalRead(2))
            {
                b = "0";
            }
            else
            {
                b = "1";
            }
            print_roll_pitch_yaw();
            prev_ms = millis();
        }
    }
}