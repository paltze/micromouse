#include "ir_sensor.h"
#include "motor.h"
#include "motion.h"
#include "gyro.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  GyroSetup();
  GyroCalibrate();
  MotorsSetup();
  MotionSetup();

  delay(2000); // Shortened delay for startup
}

/**
 * BLOCKING TURN FUNCTION
 * This function will not return until the turn is complete.
 */
void turnLeftBlocking(float targetRadians) {
  RotationTracker tracker(targetRadians);
  tracker.init();

  Serial.println("Starting Turn...");

  while (!tracker.done()) {
    float correction = tracker.tick();
    
    // Perform the movement
    turnLeft((int)correction);
    
    // Small delay to prevent CPU hogging and allow sensors to sample
    // Adjust this based on your PID/Gyro update rate requirements
    delay(10); 
  }

  coastMotion();
  Serial.println("Turn Complete.");
}


void loop() {
  // 1. Execute the turn. The program "stalls" here until finished.
  turnLeftBlocking(3.14159 / 2); // 90 degree turn

  // 2. The code only reaches this point once the turn is done.
  Serial.println("Final destination reached. Stopping forever.");
  
  while(1) {
    // Hang here to prevent the turn from repeating
    delay(1000);
  }
}