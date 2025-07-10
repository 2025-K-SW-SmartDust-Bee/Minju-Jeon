#include "FS.h"
#include "SPIFFS.h"
#include "LoRaWan_APP.h"
#include "sensor.h"
#include "lora.h"

String deviceIdStr;
const char* DEVICE_ID;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 고유 ID 생성
  uint64_t chipid = ESP.getEfuseMac();
  char idBuffer[32];
  snprintf(idBuffer, sizeof(idBuffer), "NODE_%04X%08X", (uint16_t)(chipid >> 32), (uint32_t)chipid);

  // String을 const char*로 변환
  deviceIdStr = String(idBuffer);
  DEVICE_ID = deviceIdStr.c_str();

  Serial.printf("📦 System initializing... (%s)\n", DEVICE_ID);

  if (!SPIFFS.begin(true)) {
    Serial.println("❌ Failed to initialize SPIFFS");
    while (1);
  }

  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
  setupLoRa();
  updateDetectionTarget(SENSOR_TEMPERATURE);

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
