#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

enum SensorType {
  SENSOR_NONE,
  SENSOR_TEMPERATURE,
  SENSOR_ILLUMINANCE,
  SENSOR_HUMIDITY
};

extern SensorType currentSensor;

void updateDetectionTarget(SensorType target);
void deactivateAllSensors();
String readFromCurrentSensor();

#endif
