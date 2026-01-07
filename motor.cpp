/*
 * ESP32 Wheel Speed Measurement with TB6612FNG Motor Driver
 * Groove Coupler Speed Sensor + Motor Control
 * Compatible with ESP32 Arduino Core 3.x
 * 
 * TB6612FNG Connections:
 * PWMA -> GPIO 25 | AIN1 -> GPIO 26 | AIN2 -> GPIO 27
 * PWMB -> GPIO 32 | BIN1 -> GPIO 33 | BIN2 -> GPIO 14
 * STBY -> GPIO 13 | VCC -> 3.3V | VM -> Battery+ | GND -> GND
 * 
 * Speed Sensor Connections:
 * Motor A Encoder -> GPIO 34 (ADC1)
 * Motor B Encoder -> GPIO 35 (ADC1)
 * VCC -> 3.3V/5V | GND -> GND
 */

#include <Arduino.h>
#include "motor.h"

// ===== TB6612FNG Motor Driver Pins =====
const int PWMA = 25;   // Motor A speed control
const int AIN1 = 26;   // Motor A direction pin 1
const int AIN2 = 27;   // Motor A direction pin 2
const int PWMB = 32;   // Motor B speed control
const int BIN1 = 33;   // Motor B direction pin 1
const int BIN2 = 14;   // Motor B direction pin 2
const int STBY = 13;   // Standby pin

// ===== Speed Sensor Pins =====
const int ENCODER_A = 34;  // Motor A encoder
const int ENCODER_B = 35;  // Motor B encoder

// ===== PWM Configuration =====
const int PWM_FREQ = 5000;      // 5 KHz
const int PWM_RESOLUTION = 8;   // 8-bit resolution (0-255)

// ===== Speed Measurement Variables =====
volatile unsigned long pulseCountA = 0;
volatile unsigned long pulseCountB = 0;
unsigned long lastTimeA = 0;
unsigned long lastTimeB = 0;
float rpmA = 0;
float rpmB = 0;

// Speed sensor configuration
const int SLOTS_IN_DISC = 20;  // Number of slots in encoder disc (adjust for your sensor)
const unsigned long CALC_INTERVAL = 1000;  // Calculate RPM every 1000ms

// Motor speed (0-255)
int motorSpeed = 150;

// ===== Interrupt Service Routines =====
void IRAM_ATTR encoderISR_A() {
  pulseCountA = pulseCountA + 1;
}

void IRAM_ATTR encoderISR_B() {
  pulseCountB = pulseCountB + 1;
}

// ===== Motor Control Functions =====
void setupMotors() {
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
}

void motorA(int speed) {
  // Speed range: -255 to 255
  // Positive = forward, Negative = backward, 0 = stop
  
  if (speed > 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    ledcWrite(PWMA, speed);
  } else if (speed < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    ledcWrite(PWMA, abs(speed));
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    ledcWrite(PWMA, 0);
  }
}

void motorB(int speed) {
  if (speed > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    ledcWrite(PWMB, speed);
  } else if (speed < 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    ledcWrite(PWMB, abs(speed));
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    ledcWrite(PWMB, 0);
  }
}

void stopMotors() {
  motorA(0);
  motorB(0);
}

void standby() {
  digitalWrite(STBY, LOW);  // Put motor driver in standby mode
}

void wakeup() {
  digitalWrite(STBY, HIGH);  // Wake up motor driver
}

// ===== Speed Calculation =====
void calculateSpeed() {
  unsigned long currentTime = millis();
  
  // Calculate Motor A RPM
  if (currentTime - lastTimeA >= CALC_INTERVAL) {
    noInterrupts();
    unsigned long pulses = pulseCountA;
    pulseCountA = 0;
    interrupts();
    
    float timeElapsed = (currentTime - lastTimeA) / 1000.0;  // Convert to seconds
    rpmA = (pulses / (float)SLOTS_IN_DISC) * 60.0 / timeElapsed;
    lastTimeA = currentTime;
  }
  
  // Calculate Motor B RPM
  if (currentTime - lastTimeB >= CALC_INTERVAL) {
    noInterrupts();
    unsigned long pulses = pulseCountB;
    pulseCountB = 0;
    interrupts();
    
    float timeElapsed = (currentTime - lastTimeB) / 1000.0;
    rpmB = (pulses / (float)SLOTS_IN_DISC) * 60.0 / timeElapsed;
    lastTimeB = currentTime;
  }
}

// ===== Setup =====
void MotorsSetup() {
  // Setup motor driver
  setupMotors();
  
  // Setup speed sensors
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  
  // Attach interrupts for encoders (FALLING edge for most groove coupler sensors)
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoderISR_A, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B), encoderISR_B, FALLING);
  
  lastTimeA = millis();
  lastTimeB = millis();
}

void moveForward() {
  motorA(motorSpeed);
  motorB(motorSpeed);
}

void moveBackward() {
  motorA(-motorSpeed);
  motorB(-motorSpeed);
}

void turnLeft() {
  motorA(motorSpeed / 2);
  motorB(motorSpeed);
}

void turnRight() {
  motorA(motorSpeed);
  motorB(motorSpeed / 2);
}

void stopMotion() {
  stopMotors();
}
