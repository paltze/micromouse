#include "ir_sensor.h"
#include "motor.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  MotorsSetup();
}

void loop() {
  IRSensorUpdate();

  moveForward();

  Serial.println(IRSensorIsWall(FRONT_LEFT));
}
