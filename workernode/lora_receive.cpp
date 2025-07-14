#include "lora_receive.h"
#include "sensor.h"
#include "LoRaWan_APP.h"
#include "device_id.h"
#include "lora_send.h"

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
  char rxBuf[256];
  memcpy(rxBuf, payload, size);
  rxBuf[size] = '\0';

  if (strcmp(rxBuf, "ACK") == 0) {
    Serial.printf("Received ACK | RSSI: %d\n", rssi);
    Radio.Rx(0);
    lora_idle = true;
    return;
  }

  if (strstr(rxBuf, DEVICE_ID) != nullptr) {
    Serial.println("Skipping self-packet");
    Radio.Rx(0);
    lora_idle = true;
    return;
  }

  String message = String(rxBuf);
  int startIdx = message.indexOf('[');
  int midIdx = message.indexOf('|');
  int endIdx = message.indexOf(']');

  if (startIdx != -1 && midIdx != -1 && endIdx != -1) {
    String senderId = message.substring(startIdx + 1, midIdx);
    String msgNumber = message.substring(midIdx + 1, endIdx);
    String content = message.substring(endIdx + 2);
    Serial.printf("From %s | Msg #%s | Content: %s | RSSI: %d\n",
                  senderId.c_str(), msgNumber.c_str(), content.c_str(), rssi);
  } else {
    Serial.printf("Unknown Format: %s | RSSI: %d\n", rxBuf, rssi);
  }

  Serial.println("Sending ACK...");
  Radio.Send((uint8_t*)"ACK", 3);
  delay(100);
  Radio.Rx(0);
  lora_idle = true;
}
