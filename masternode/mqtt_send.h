#ifndef MQTT_SEND_H
#define MQTT_SEND_H

#include <WiFi.h>
#include <PubSubClient.h>

extern PubSubClient client;

void setup_wifi(const char* ssid, const char* password);
void reconnect(PubSubClient& client);
void callback(char* topic, byte* payload, unsigned int length);

#endif