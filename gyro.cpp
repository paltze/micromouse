#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include "gyro.h"

#define SDA 21
#define SCL 22

Adafruit_MPU6050 mpu;

sensors_event_t a, g, temp;

void GyroSetup() {
    Wire.begin(SDA, SCL);

    if (!mpu.begin()) {
        while (1) delay(10);
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

float gyroX_bias = 0;
float gyroY_bias = 0;
float gyroZ_bias = 0;

void GyroCalibrate() {
    const int samples = 500;

    float sumX = 0;
    float sumY = 0;
    float sumZ = 0;

    for (int i = 0; i < samples; i++) {
        mpu.getEvent(&a, &g, &temp);

        sumX += g.gyro.x;
        sumY += g.gyro.y;
        sumZ += g.gyro.z;

        delay(2);
    }

    gyroX_bias = sumX / samples;
    gyroY_bias = sumY / samples;
    gyroZ_bias = sumZ / samples;
}

void GyroUpdate() {
    mpu.getEvent(&a, &g, &temp);
}

float GyroX() {
    return g.gyro.x - gyroX_bias;
}

float GyroY() {
    return g.gyro.y - gyroY_bias;
}

float GyroZ() {
    return g.gyro.z - gyroZ_bias;
}
