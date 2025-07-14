#include "sensor.h"

DHT dht(DHTPIN, DHTTYPE);
SensorType currentSensor = SENSOR_TEMPERATURE;

void initSensors() {
  dht.begin();
}

void updateDetectionTarget(SensorType target) {
  deactivateAllSensors();
  currentSensor = target;
  switch (target) {
    case SENSOR_TEMPERATURE: Serial.println("Temp sensor activated"); break;
    case SENSOR_ILLUMINANCE: Serial.println("Light sensor activated"); break;
    case SENSOR_HUMIDITY:    Serial.println("Humidity sensor activated"); break;
    default:                 Serial.println("No valid sensor selected"); break;
  }
}

void deactivateAllSensors() {
  Serial.println("All sensors deactivated");
}

String readAllSensors() {
  String result = "";

  float t = dht.readTemperature();
  if (isnan(t)) result += "Temp: error\n";
  else result += "Temp: " + String(t, 1) + "C\n";

  float h = dht.readHumidity();
  if (isnan(h)) result += "Humidity: error\n";
  else result += "Humidity: " + String(h, 1) + "%\n";

  int light = analogRead(PHOTO_PIN);
  result += "Light: " + String(light) + "/4095\n";

  return result;
}

