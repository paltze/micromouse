#ifndef MOTION_H
#define MOTION_H

class DistanceTracker {
  unsigned long lastPulse;
  float distance;
  int target;
  float distPerPulse;

  long lastL = 0;
  long lastR = 0;
  float lastError = 0;

public:
  DistanceTracker(int radius, int target);
  void init();
  float tick();
  bool done();
};

class RotationTracker {
  float angle;
  float target;

  unsigned long lastTime;
  float lastError;

public:
  RotationTracker(float target);
  void init();
  float tick();
  bool done();
};

void MotionSetup();

long unsigned pulseL();
long unsigned pulseR();

#endif
