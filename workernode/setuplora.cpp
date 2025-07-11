#include "setuplora.h"
#include "lora_send.h"
#include "lora_receive.h"
#include "LoRaWan_APP.h"

static RadioEvents_t RadioEvents;

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
