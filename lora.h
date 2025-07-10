#ifndef LORA_H
#define LORA_H

#include <Arduino.h>
#include "sensor.h"

void setupLoRa();
void handleLoRaEvents();
void tryTransmit();

#endif
