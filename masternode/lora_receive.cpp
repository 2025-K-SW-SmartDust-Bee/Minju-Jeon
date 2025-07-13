#include "lora_receive.h"
#include "sensor.h"
#include "LoRaWan_APP.h"
#include "device_id.h"
#include "lora_send.h"
#include "mqtt_send.h"

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

  String formattedMessage;

  if (startIdx != -1 && midIdx != -1 && endIdx != -1) {
    String senderId = message.substring(startIdx + 1, midIdx);
    String msgNumber = message.substring(midIdx + 1, endIdx);
    String content = message.substring(endIdx + 2);
    Serial.printf("📥 From %s | Msg #%s | Content: %s | RSSI: %d\n",
                  senderId.c_str(), msgNumber.c_str(), content.c_str(), rssi);
    formattedMessage = 
    "{\"ID\":\"" + senderId + 
      "\",\"msgNumber\":\"" + msgNumber + 
      "\",\"content\":\"" + content + 
      "\",\"RSSI\":\"" + String(rssi) +"\"}";
  } else {
    Serial.printf("📥 Unknown Format: %s | RSSI: %d\n", rxBuf, rssi);
      formattedMessage ="{\"Unknown Format\":\"" + String(rxBuf) +
        "\",\"RSSI\":\"" + String(rssi) + "\"";
  }

  // sent to MQTT via wifi
  Serial.printf("Message to MQTT Server %s \n", formattedMessage.c_str());
  client.publish("farm/sensor", formattedMessage.c_str());
  Serial.print("📨 Sending to MQTT Server...");

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
