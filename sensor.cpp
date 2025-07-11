#include "sensor.h"

SensorType currentSensor = SENSOR_TEMPERATURE;

void updateDetectionTarget(SensorType target) {
  deactivateAllSensors();
  currentSensor = target;

  switch (target) {
    case SENSOR_TEMPERATURE: Serial.println("🌡️ Temp sensor activated"); break;
    case SENSOR_ILLUMINANCE: Serial.println("💡 Light sensor activated"); break;
    case SENSOR_HUMIDITY:    Serial.println("💧 Humidity sensor activated"); break;
    default:                 Serial.println("❌ No valid sensor selected"); break;
  }
}

void deactivateAllSensors() {
  Serial.println("🔌 All sensors deactivated");
}

String readFromCurrentSensor() {
  int value = random(0, 100);
  switch (currentSensor) {
    case SENSOR_TEMPERATURE: return "Temp: " + String(value) + "C";
    case SENSOR_ILLUMINANCE: return "Light: " + String(value) + "lx";
    case SENSOR_HUMIDITY:    return "Humidity: " + String(value) + "%";
    default: return "No active sensor";
  }
}
