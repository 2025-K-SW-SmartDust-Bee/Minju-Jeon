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
  if (currentSensor == SENSOR_TEMPERATURE) {
    float t = dht.readTemperature();
    if (isnan(t)) return "Temp: error";
    return "Temp: " + String(t, 1) + "C";
  } else if (currentSensor == SENSOR_HUMIDITY) {
    float h = dht.readHumidity();
    if (isnan(h)) return "Humidity: error";
    return "Humidity: " + String(h, 1) + "%";
  } else if (currentSensor == SENSOR_ILLUMINANCE) {
    int value = analogRead(PHOTO_PIN); // 조도값 읽기
    return "Light: " + String(value) + "/4095";
  } else {
    return "No active sensor";
  }
}
