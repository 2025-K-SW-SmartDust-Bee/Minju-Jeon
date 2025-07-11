#pragma once

#include <Arduino.h>

extern char txpacket[64];
extern bool lora_idle;
extern double txNumber;
extern unsigned long lastSendTime;
extern unsigned long nextSendInterval;

void OnTxDone();
void OnTxTimeout();
void tryTransmit();
