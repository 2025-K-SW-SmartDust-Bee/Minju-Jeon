#include "lora_receive.h"
#include "sensor.h"
#include "LoRaWan_APP.h"
#include "device_id.h"
#include "lora_send.h"

#include <ArduinoJson.h>  // 꼭 include

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
  char rxBuf[256];
  memset(rxBuf, 0, sizeof(rxBuf));
  memcpy(rxBuf, payload, size);
  rxBuf[size] = '\0';

  Serial.printf("📦 수신된 원문:\n%s\n", rxBuf);

  if (strcmp(rxBuf, "ACK") == 0) {
    Serial.printf("Received ACK | RSSI: %d\n", rssi);
    Radio.Rx(0);
    lora_idle = true;
    return;
  }

  // 자신이 보낸 패킷 무시
  if (strstr(rxBuf, DEVICE_ID) != nullptr) {
    Serial.println("Skipping self-packet");
    Radio.Rx(0);
    lora_idle = true;
    return;
  }

  // JSON 파싱 시도
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, rxBuf);

  if (!error) {
    // ✅ JSON 성공 처리
    const char* source = doc["source"];
    const char* type = doc["type"];
    float temp = doc["payload"]["temperature"];
    float humidity = doc["payload"]["humidity"];
    int light = doc["payload"]["light"];

    Serial.printf("📥 JSON 수신 from %s (%s):\n", source, type);
    Serial.printf("🌡️ Temp: %.1f°C | 💧 Humidity: %.1f%% | 💡 Light: %d | RSSI: %d\n", temp, humidity, light, rssi);
  } else {
    // ⚠️ JSON이 아닌 일반 메시지 처리
    Serial.printf("⚠️ JSON 파싱 실패: %s\n", error.c_str());

    // 기존 텍스트 메시지 처리
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
  }

  Serial.println("Sending ACK...");
  Radio.Send((uint8_t*)"ACK", 3);
  delay(100);
  Radio.Rx(0);
  lora_idle = true;
}
