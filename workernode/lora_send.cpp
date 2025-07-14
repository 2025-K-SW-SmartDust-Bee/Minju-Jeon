#include "lora_send.h"
#include "sensor.h"
#include "LoRaWan_APP.h"
#include "device_id.h"

char txpacket[256];
bool lora_idle = true;
double txNumber = 0;
unsigned long lastSendTime = 0;
unsigned long nextSendInterval = 10000;

void OnTxDone() {
  Serial.println("LoRa TX done");
  Radio.Rx(0);
  lora_idle = true;
}

void OnTxTimeout() {
  Serial.println("LoRa TX timeout");
  Radio.Rx(0);
  lora_idle = true;
}

void tryTransmit() {
  if (millis() - lastSendTime >= nextSendInterval && lora_idle) {
    delay(random(100, 500));

    String data = readAllSensors();
    if (data.length() > 400) {
      Serial.println("❗ 센서 데이터가 너무 깁니다. 전송 생략");
      return;
    }

    int len = snprintf(txpacket, sizeof(txpacket), "[%s|%0.0f] %s", DEVICE_ID, ++txNumber, data.c_str());
    if (len < 0 || len >= sizeof(txpacket)) {
      Serial.println("❌ snprintf overflow 발생!");
      return;
    }

    Serial.printf("Sending (%d bytes):\n%s\n", len, txpacket);

    Radio.Send((uint8_t*)txpacket, strlen(txpacket));

    lora_idle = false;
    lastSendTime = millis();
    nextSendInterval = random(8000, 15000);
  }
}

