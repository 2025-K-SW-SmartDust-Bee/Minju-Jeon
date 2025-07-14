#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 19
#define DHTTYPE DHT22
#define PHOTO_PIN 20

enum SensorType {
  SENSOR_NONE,
  SENSOR_TEMPERATURE,
  SENSOR_ILLUMINANCE,
  SENSOR_HUMIDITY
};

extern DHT dht;
extern SensorType currentSensor;

void initSensors();
void updateDetectionTarget(SensorType target);
void deactivateAllSensors();
String readFromCurrentSensor();
String readAllSensors();

#endif
