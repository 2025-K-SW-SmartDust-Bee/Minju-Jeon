#include "device_id.h"

String deviceIdStr;
const char* DEVICE_ID;

void initDeviceId() {
  uint64_t chipid = ESP.getEfuseMac();
  char idBuffer[32];
  snprintf(idBuffer, sizeof(idBuffer), "NODE_%04X%08X", (uint16_t)(chipid >> 32), (uint32_t)chipid);

  deviceIdStr = String(idBuffer);
  DEVICE_ID = deviceIdStr.c_str();
}
