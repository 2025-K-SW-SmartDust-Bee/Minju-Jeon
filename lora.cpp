#include "lora.h"
#include "LoRaWan_APP.h"

extern const char* DEVICE_ID;

char txpacket[64];
bool lora_idle = true;
double txNumber = 0;

unsigned long lastSendTime = 0;
unsigned long nextSendInterval = 10000;

static RadioEvents_t RadioEvents;

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

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
  char rxBuf[64];
  memcpy(rxBuf, payload, size);
  rxBuf[size] = '\0';

  if (strcmp(rxBuf, "ACK") == 0) {
    Serial.printf("📥 Received ACK | RSSI: %d\n", rssi);
    Radio.Rx(0);
    lora_idle = true;
    return;
  }

  if (strstr(rxBuf, DEVICE_ID) != nullptr) {
    Serial.println("🛑 Skipping self-packet");
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
    Serial.printf("📥 From %s | Msg #%s | Content: %s | RSSI: %d\n",
                  senderId.c_str(), msgNumber.c_str(), content.c_str(), rssi);
  } else {
    Serial.printf("📥 Unknown Format: %s | RSSI: %d\n", rxBuf, rssi);
  }

  if (strstr(rxBuf, "Temp") != nullptr) updateDetectionTarget(SENSOR_TEMPERATURE);
  else if (strstr(rxBuf, "Light") != nullptr) updateDetectionTarget(SENSOR_ILLUMINANCE);
  else if (strstr(rxBuf, "Humidity") != nullptr) updateDetectionTarget(SENSOR_HUMIDITY);
  else if (strstr(rxBuf, "None") != nullptr) updateDetectionTarget(SENSOR_NONE);

  Serial.println("📨 Sending ACK...");
  Radio.Send((uint8_t*)"ACK", 3);
  delay(100);
  Radio.Rx(0);
  lora_idle = true;
}

void setupLoRa() {
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxDone = OnRxDone;
  Radio.Init(&RadioEvents);

  Radio.SetChannel(915000000);
  Radio.SetTxConfig(MODEM_LORA, 5, 0, 0, 7, 1, 8,
                    false, false, 0, 0, false, 3000);
  Radio.SetRxConfig(MODEM_LORA, 0, 7, 1, 0, 8, 0,
                    false, 0, true, 0, 0, false, true);

  Radio.Rx(0);
}

void handleLoRaEvents() {
  Radio.IrqProcess();
}

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