#include "FS.h"
#include "SPIFFS.h"
#include "LoRaWan_APP.h"
#include "sensor.h"
#include "device_id.h"
#include "setuplora.h"
#include "lora_send.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("▶ setup() 시작");

  initDeviceId();
  initSensors();
  
  Serial.printf("System initializing... (%s)\n", DEVICE_ID);

  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to initialize SPIFFS");
    while (1);
  }
  Serial.println("SPIFFS OK");

  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
  Serial.println("Mcu.begin() OK");

  setupLoRa();
  Serial.println("setupLoRa() OK");
  Serial.println("센서 OK");
  Serial.println("System ready");
}


void loop() {
  handleLoRaEvents();

  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'a') {  // 모든 센서 값 확인
      Serial.println("Reading all sensors...");
      Serial.println(readAllSensors());
    }
  }

  tryTransmit();
}
