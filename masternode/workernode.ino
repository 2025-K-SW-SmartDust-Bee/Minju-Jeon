#include "FS.h"
#include "SPIFFS.h"
#include "LoRaWan_APP.h"
#include "sensor.h"
#include "device_id.h"
#include "setuplora.h"
#include "lora_send.h"
#include "mqtt_send.h"
#include <WiFi.h>
#include <PubSubClient.h>

// Wifi and MQTT Server Configuration
const char* ssid = "IITP";
const char* password = "IITPWireless!25";
const char* mqtt_server = "43.135.166.229";  // 你的 MQTT 服务器 IP

WiFiClient espClient;
PubSubClient client(espClient);

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

  setup_wifi(ssid, password);
  Serial.println("\n✅ Wifi Configuration Done");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Serial.println("✅ MQTT Server Set");

  Serial.println("✅ System ready");
}


void loop() {
  handleLoRaEvents();

  // Check if Wifi is connected
  if (!client.connected()) {
    reconnect(client);
  }
  client.loop();

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
