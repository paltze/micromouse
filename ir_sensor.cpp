#include <Arduino.h>
#include "ir_sensor.h"

#define FRONT_LEFT_PIN 17
#define FRONT_LEFT_FAR_PIN 4
#define FRONT_RIGHT_PIN 16
#define FRONT_RIGHT_FAR_PIN 5
#define REAR_PIN 15

IRSensor::IRSensor(int pin) {
  this->pin = pin;
}

void IRSensor::update() {
  this->detected = !digitalRead(this->pin);
}

bool IRSensor::isDetected() {
  return this->detected;
}

IRSensor frontLeft(FRONT_LEFT_PIN);
IRSensor frontLeftFar(FRONT_LEFT_FAR_PIN);
IRSensor frontRight(FRONT_RIGHT_PIN);
IRSensor frontRightFar(FRONT_RIGHT_FAR_PIN);
IRSensor rear(REAR_PIN);

void IRSensorUpdate() {
  frontLeft.update();
  frontLeftFar.update();
  frontRight.update();
  frontRightFar.update();
  rear.update();
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
    case REAR:
      return rear.isDetected();
      break;
  }
  return false;
}
