#include <Arduino.h>
#include "ir_sensor.h"

#define THRESHOLD 1500

#define FRONT_LEFT_PIN 39
#define FRONT_LEFT_FAR_PIN 36
#define FRONT_RIGHT_PIN 34
#define FRONT_RIGHT_FAR_PIN 35
#define REAR_LEFT_PIN 32
#define REAR_RIGHT_PIN 33

IRSensor::IRSensor(int pin) {
  this->pin = pin;
}

void IRSensor::update() {
  int analogValue = analogRead(this->pin);
  this->detected = (analogValue > THRESHOLD) ? true : false;
}

bool IRSensor::isDetected() {
  return this->detected;
}

IRSensor frontLeft(FRONT_LEFT_PIN);
IRSensor frontLeftFar(FRONT_LEFT_FAR_PIN);
IRSensor frontRight(FRONT_RIGHT_PIN);
IRSensor frontRightFar(FRONT_RIGHT_FAR_PIN);
IRSensor rearLeft(REAR_LEFT_PIN);
IRSensor rearRight(REAR_RIGHT_PIN);

void IRSensorUpdate() {
  frontLeft.update();
  frontLeftFar.update();
  frontRight.update();
  frontRightFar.update();
  rearLeft.update();
  rearRight.update();
}

bool IRSensorIsWall(IR_SENSOR sensor) {
  switch(sensor) {
    case FRONT_LEFT:
      return frontLeft.isDetected();
      break;
    case FRONT_LEFT_FAR:
      return frontLeftFar.isDetected();
      break;
    case FRONT_RIGHT:
      return frontRight.isDetected();
      break;
    case FRONT_RIGHT_FAR:
      return frontRightFar.isDetected();
      break;
    case REAR_LEFT:
      return rearLeft.isDetected();
      break;
    case REAR_RIGHT:
      return rearRight.isDetected();
      break;
  }
}
