#include "ir_sensor.h"
#include "motor.h"

#include <WiFi.h>
#include <esp_bt.h>

int motorState = 0;

void setup() {
  WiFi.mode(WIFI_OFF);
  btStop();
  esp_bt_controller_disable();


  Serial.begin(115200);
  delay(1000);

  MotorsSetup();

  Serial.println("Serial working");
}

void loop() {
  /*if (Serial.available() > 0) {
    char cmd = Serial.read();

    switch (cmd) {
      case 'f':
        moveForward();
        break;

      case 'b':
        moveBackward();
        break;

      case 'l':
        turnLeft();
        break;

      case 'r':
        turnRight();
        break;

      case 's':
        stopMotion();
        break;

      default:
        // ignore junk input
        break;
    }
  }*/



  /*delay(5000);

  IRSensorUpdate();

  Serial.print("FRONT_LEFT: ");
  Serial.println(IRSensorIsWall(FRONT_LEFT));

  Serial.print("FRONT_LEFT_FAR: ");
  Serial.println(IRSensorIsWall(FRONT_LEFT_FAR));

  Serial.print("FRONT_RIGHT: ");
  Serial.println(IRSensorIsWall(FRONT_RIGHT));

  Serial.print("FRONT_RIGHT_FAR: ");
  Serial.println(IRSensorIsWall(FRONT_RIGHT_FAR));

  Serial.print("REAR_LEFT: ");
  Serial.println(IRSensorIsWall(REAR_LEFT));

  Serial.print("REAR_RIGHT: ");
  Serial.println(IRSensorIsWall(REAR_RIGHT));*/

  /*if (IRSensorIsWall(FRONT_LEFT) || IRSensorIsWall(FRONT_RIGHT)) {
    stopMotion();
  }
  else {
    moveForward();
  }*/

  IRSensorUpdate();

  if ( !motorState && ( !IRSensorIsWall(FRONT_LEFT) && !IRSensorIsWall(FRONT_RIGHT) ) ) {
    moveForward();
    motorState = 1;
  }

  if ( motorState && ( !IRSensorIsWall(REAR_LEFT) && !IRSensorIsWall(REAR_RIGHT) ) ) {
    moveBackward();
    motorState = 0;
  }

  /*if (motorState && ( IRSensorIsWall(FRONT_LEFT) || IRSensorIsWall(FRONT_RIGHT) )) {
    moveBackward();
    motorState = 0;
  }*/
  
  delay(200);
}

