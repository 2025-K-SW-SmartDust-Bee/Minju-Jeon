#pragma once

#include <Arduino.h>

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
