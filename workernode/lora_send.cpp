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

void sendLoRaPacket(const String& data) {
  if (data.length() > sizeof(txpacket) - 1) {
    Serial.println("❗ 데이터가 너무 깁니다. 전송 생략");
    return;
  }

  data.toCharArray(txpacket, sizeof(txpacket));
  Serial.printf("Sending (%d bytes):\n%s\n", data.length(), txpacket);

  Radio.Send((uint8_t*)txpacket, strlen(txpacket));

  lora_idle = false;
  lastSendTime = millis();
  nextSendInterval = random(8000, 15000);
}

void tryTransmitAllSensors() {
  if (millis() - lastSendTime >= nextSendInterval && lora_idle) {
    delay(random(100, 500));
    String jsonData = readAllSensorsAsJson();
    sendLoRaPacket(jsonData);
  }
}

void tryTransmitCurrentSensor() {
  if (millis() - lastSendTime >= nextSendInterval && lora_idle) {
    delay(random(100, 500));
    String data = readFromCurrentSensor();
    sendLoRaPacket(data);
  }
}