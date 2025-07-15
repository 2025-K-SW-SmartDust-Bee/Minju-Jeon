#include "sensor.h"
#include <ArduinoJson.h>
#include "device_id.h"

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

String readAllSensorsAsJson() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int light = analogRead(PHOTO_PIN);

  StaticJsonDocument<256> doc;
  doc["type"] = "periodically";

  if (temperature > 36 || humidity > 70) {
    doc["type"] = "emergency";
  }

   // should be changed by situation
  doc["source"] = String("sensor/") + DEVICE_ID; 

  JsonObject payload = doc.createNestedObject("payload");

  if (!isnan(temperature)) payload["temperature"] = temperature;
  if (!isnan(humidity))    payload["humidity"] = humidity;
  payload["light"] = light;

  String output;
  serializeJson(doc, output);
  return output;
}

