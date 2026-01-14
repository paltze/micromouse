#ifndef MOTOR_H
#define MOTOR_H

void MotorsSetup();

void setSpeed(int speed);

//void MotorsStandby();
//void MotorsWakeup();

void moveForward(float correction);
void moveBackward(float correction);
void turnLeft(float correction);
void turnRight(float correction);
void brakeMotion();
void coastMotion();

#endif
