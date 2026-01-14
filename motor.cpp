/*
 * ESP32 Wheel Speed Measurement with TB6612FNG Motor Driver
 * Groove Coupler Speed Sensor + Motor Control
 * Compatible with ESP32 Arduino Core 3.x
 * 
 * TB6612FNG Connections:
 * PWMA -> GPIO 23 | AIN1 -> GPIO 25 | AIN2 -> GPIO 26
 * PWMB -> GPIO 27 | BIN1 -> GPIO 14 | BIN2 -> GPIO 16
 * STBY -> GPIO 13 | VCC -> 3.3V | VM -> Battery+ | GND -> GND
 * 
 * Speed Sensor Connections:
 * Motor A Encoder -> GPIO 18 (ADC1)
 * Motor B Encoder -> GPIO 19 (ADC1)
 * VCC -> 3.3V/5V | GND -> GND 
 */

#include <Arduino.h>
#include "motor.h"

// ===== TB6612FNG Motor Driver Pins =====
/*const int PWMA = 23;   // Motor A speed control
const int AIN1 = 25;   // Motor A direction pin 1
const int AIN2 = 26;   // Motor A direction pin 2
const int PWMB = 27;   // Motor B speed control
const int BIN1 = 14;   // Motor B direction pin 1
const int BIN2 = 16;   // Motor B direction pin 2
const int STBY = 13;   // Standby pin*/

// ===== PWM Configuration =====
const int PWM_FREQ = 1000;      // 5 KHz
const int PWM_RESOLUTION = 8;   // 8-bit resolution (0-255)

// Motor speed (0-255)
int motorSpeed = 255;

// ===== Motor Control Functions =====
/*void MotorsSetup() {
  // Configure PWM for Motor A (ESP32 Core 3.x)
  ledcAttach(PWMA, PWM_FREQ, PWM_RESOLUTION);
  
  // Configure PWM for Motor B (ESP32 Core 3.x)
  ledcAttach(PWMB, PWM_FREQ, PWM_RESOLUTION);
  
  // Configure direction pins
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  
  // Enable motor driver (STBY HIGH)
  digitalWrite(STBY, HIGH);
}*/

// ===== L298N Motor Driver Pins =====
const int ENA = 23;   // Motor A speed (PWM)
const int IN1 = 25;   // Motor A direction
const int IN2 = 26;

const int ENB = 27;   // Motor B speed (PWM)
const int IN3 = 14;   // Motor B direction
const int IN4 = 13;

void MotorsSetup() {
  ledcAttach(ENA, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(ENB, PWM_FREQ, PWM_RESOLUTION);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

/*void motorA(int speed) {
  // Speed range: -255 to 255
  // Positive = forward, Negative = backward, 0 = stop
  
  if (speed > 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    ledcWrite(PWMA, speed);
  } else if (speed < 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    ledcWrite(PWMA, abs(speed));
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    ledcWrite(PWMA, 0);
  }
}*/

/*void motorB(int speed) {
  if (speed > 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    ledcWrite(PWMB, speed);
  } else if (speed < 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    ledcWrite(PWMB, abs(speed));
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    ledcWrite(PWMB, 0);
  }
}

void MotorsStandby() {
  digitalWrite(STBY, LOW);  // Put motor driver in standby mode
}

void MotorsWakeup() {
  digitalWrite(STBY, HIGH);  // Wake up motor driver
}*/

void motorAStop() {
  // Soft brake
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  ledcWrite(ENA, motorSpeed/2);     // light brake
  delay(15);              // 10–20 ms is enough

  // Coast
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  ledcWrite(ENA, 0);
}

void motorBStop() {
  // Soft brake
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
  ledcWrite(ENB, motorSpeed/2);     // light brake
  delay(15);              // 10–20 ms is enough

  // Coast
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  ledcWrite(ENB, 0);
}

void motorA(int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    ledcWrite(ENA, speed);
  } 
  else if (speed < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    ledcWrite(ENA, -speed);
  } 
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    ledcWrite(ENA, 0);
  }
}

void motorB(int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    ledcWrite(ENB, speed);
  } 
  else if (speed < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    ledcWrite(ENB, -speed);
  } 
  else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    ledcWrite(ENB, 0);
  }
}


void setSpeed(int speed) {
  motorSpeed = speed;
}

void moveForward(float correction) {
  correction = constrain(correction, -1 * (0.3 * motorSpeed), 0.3 * motorSpeed);
  motorA(motorSpeed + correction);
  motorB(motorSpeed - correction);
}

void moveBackward(float correction) {
  correction = constrain(correction, -1 * (0.3 * motorSpeed), 0.3 * motorSpeed);
  motorA(-motorSpeed + correction);
  motorB(-motorSpeed - correction);
}

void turnLeft(float correction) {
  int speed = motorSpeed / 4;
  correction = constrain(correction, -1 * (0.3 * speed), 0.3 * speed);
  motorA(-speed - correction);
  motorB(speed + correction);
}

void turnRight(float correction) {
  int speed = motorSpeed / 4;
  correction = constrain(correction, -1 * (0.3 * speed), 0.3 * speed);
  motorA(speed + correction);
  motorB(-speed - correction);
}

void brakeMotion() {
  motorAStop();
  motorBStop();
}

void coastMotion() {
  motorA(0);
  motorB(0);
}
