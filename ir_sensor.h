#ifndef IR_SENSOR_H
#define IR_SENSOR_H

typedef enum {
  FRONT_LEFT,
  FRONT_LEFT_FAR,
  FRONT_RIGHT,
  FRONT_RIGHT_FAR,
  REAR_LEFT,
  REAR_RIGHT
} IR_SENSOR;

class IRSensor {
  int pin;
  bool detected;

  public:
  IRSensor(int pin);
  void update();
  bool isDetected();
};

void IRSensorUpdate();
bool IRSensorIsWall(IR_SENSOR sensor);

#endif
