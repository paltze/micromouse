#include <Arduino.h>
#include "motion.h"

#include "gyro.h"

//Pulses per 10 rev: L: 34160, R: 36599

#define ENCODER_LEFT_A_PIN 18
//#define ENCODER_LEFT_B_PIN 19

#define ENCODER_RIGHT_A_PIN 19
//#define ENCODER_RIGHT_B_PIN 33

volatile long unsigned pulseCountL = 0, pulseCountR = 0;

float Kp = 0.01;
float Kd = 0.05;

DistanceTracker::DistanceTracker(int radius, int target) {
  this->target = target;
  this->distPerPulse = (2.0 * 2.0 * 3.14159 * radius) / (3660 + 3416);
}

void DistanceTracker::init() {
  this->distance = 0;
  this->lastError = 0;
  
  // Initialize pulse tracking to current values to avoid a massive spike on first tick
  noInterrupts();
  this->lastPulse = (pulseCountL + pulseCountR) / 2;
  this->lastL = pulseCountL;
  this->lastR = pulseCountR;
  interrupts();
}

float DistanceTracker::tick() {
  long a, b;
  noInterrupts();
  a = pulseCountL;
  b = pulseCountR;
  interrupts();

  // 1. Update Distance
  unsigned long currentAvg = (a + b) / 2;
  unsigned long dP = currentAvg - this->lastPulse;
  this->lastPulse = currentAvg;
  this->distance += this->distPerPulse * dP;

  // 2. Calculate PD Steering (formerly PD_straight/readDeltaError)
  long dL = a - this->lastL;
  long dR = b - this->lastR;
  this->lastL = a;
  this->lastR = b;

  float error = (float)(dL - dR);
  float derivative = error - this->lastError;
  this->lastError = error;

  return (Kp * error) + (Kd * derivative);
}

bool DistanceTracker::done() {
  return this->distance >= this->target;
}

float Kp_rot = -0.01;
float Kd_rot = 0.0;

RotationTracker::RotationTracker(float target) {
  this->target = target;
}

void RotationTracker::init() {
  this->angle = 0.0f;
  this->lastError = 0.0f;
  this->lastTime = millis();
}

float RotationTracker::tick() {
  GyroUpdate();

  unsigned long now = millis();
  float dt = (now - this->lastTime) * 0.001f;
  this->lastTime = now;

  this->angle += abs(GyroZ() * dt);

  Serial.println(this->angle);

  float error = this->target - this->angle;
  float derivative = error - this->lastError;
  this->lastError = error;

  return Kp_rot * error + Kd_rot * derivative;
}

bool RotationTracker::done() {
  return this->angle >= this->target; // ~1 degree
}



void IRAM_ATTR encoderISR_L() {
  /*int A = digitalRead(ENCODER_LEFT_A_PIN);
  int B = digitalRead(ENCODER_LEFT_B_PIN);

  if (A == B) pulseCountL++;*/

  pulseCountL++;
}

void IRAM_ATTR encoderISR_R() {
  /*int A = digitalRead(ENCODER_RIGHT_A_PIN);
  int B = digitalRead(ENCODER_RIGHT_B_PIN);

  if (A == B) pulseCountR++;*/

  pulseCountR++;
}

void MotionSetup() {
  pinMode(ENCODER_LEFT_A_PIN, INPUT_PULLUP);
  //pinMode(ENCODER_LEFT_B_PIN, INPUT_PULLUP);

  pinMode(ENCODER_RIGHT_A_PIN, INPUT_PULLUP);
  //pinMode(ENCODER_RIGHT_B_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_A_PIN), encoderISR_L, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_A_PIN), encoderISR_R, RISING);
}

long unsigned pulseL() {
  return pulseCountL;
}

long unsigned pulseR() {
  return pulseCountR;
}
