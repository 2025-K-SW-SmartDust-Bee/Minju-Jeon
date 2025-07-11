#include "FS.h"
#include "SPIFFS.h"
#include "sensor.h"
#include "device_id.h"
#include "LoRaWan_APP.h"
#include "lora.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("▶ setup() 시작");

  initDeviceId();
  Serial.printf("📦 System initializing... (%s)\n", DEVICE_ID);

  if (!SPIFFS.begin(true)) {
    Serial.println("❌ Failed to initialize SPIFFS");
    while (1);
  }
  Serial.println("✅ SPIFFS OK");

  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
  Serial.println("✅ Mcu.begin() OK");

  setupLoRa();
  Serial.println("✅ setupLoRa() OK");

  updateDetectionTarget(SENSOR_TEMPERATURE);
  Serial.println("✅ 센서 OK");

  Serial.println("✅ System ready");
}


void loop() {
  handleLoRaEvents();

  if (Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case 't': updateDetectionTarget(SENSOR_TEMPERATURE); break;
      case 'l': updateDetectionTarget(SENSOR_ILLUMINANCE); break;
      case 'h': updateDetectionTarget(SENSOR_HUMIDITY); break;
      case 'n': updateDetectionTarget(SENSOR_NONE); break;
    }
  }

  tryTransmit();
}
