#include "lora.h"
#include "LoRaWan_APP.h"
#include "device_id.h"

char txpacket[64];
bool lora_idle = true;
double txNumber = 0;
unsigned long lastSendTime = 0;
unsigned long nextSendInterval = 10000;

void tryTransmit() {
  if (millis() - lastSendTime >= nextSendInterval && lora_idle) {
    delay(random(100, 500));
    String data = readFromCurrentSensor();
    snprintf(txpacket, sizeof(txpacket), "[%s|%0.0f] %s", DEVICE_ID, ++txNumber, data.c_str());
    Serial.printf("📤 Sending: %s\n", txpacket);

    Radio.Send((uint8_t*)txpacket, strlen(txpacket));
    lora_idle = false;
    lastSendTime = millis();
    nextSendInterval = random(8000, 15000);
  }
}

void OnTxDone() {
  Serial.println("✅ LoRa TX done");
  Radio.Rx(0);
  lora_idle = true;
}

void OnTxTimeout() {
  Serial.println("⏱️ LoRa TX timeout");
  Radio.Rx(0);
  lora_idle = true;
}
