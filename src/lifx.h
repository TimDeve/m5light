#pragma once

#include "lifxmessage.h"

void sendTurnOnMessage();
void sendTurnOffMessage();
void sendSetColorMessage(uint16_t brightness, uint16_t kelvin);
void setupUdp();
SetColorMessage createSetColorMessage(uint16_t brightness, uint16_t kelvin);
