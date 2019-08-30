#include <M5Stack.h>

#include "secrets.h"
#include "lifx.h"
#include "ui.h"
#include "wificonnection.h"

const char *networkName = NETWORK_NAME;
const char *networkPswd = NETWORK_PASS;

void setup()
{
  // Init lcd, serial, but not the sd card
  M5.begin(true, false, true);

  uiSetup();

  connectToWiFi(networkName, networkPswd);
}

void loop()
{
  uiLoop();
}
