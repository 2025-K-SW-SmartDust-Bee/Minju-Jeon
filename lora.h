#pragma once

#include <Arduino.h>
#include "sensor.h"

extern char txpacket[64];
extern bool lora_idle;
extern double txNumber;
extern unsigned long lastSendTime;
extern unsigned long nextSendInterval;

void OnTxDone();
void OnTxTimeout();
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

void tryTransmit();
void setupLoRa();
void handleLoRaEvents();
