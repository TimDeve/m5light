#include <M5Stack.h>

#include "secrets.h"
#include "lifx.h"
#include "ui.h"
#include "wificonnection.h"

const char *networkName = NETWORK_NAME;
const char *networkPswd = NETWORK_PASS;

void setup()
{
  uiSetup();

  connectToWiFi(networkName, networkPswd);
}

void loop()
{
  uiLoop();
}
