#include <M5Stack.h>
#include <WiFi.h>

#include "secrets.h"
#include "lifx.h"
#include "ui.h"

const char *networkName = NETWORK_NAME;
const char *networkPswd = NETWORK_PASS;

boolean isWifiConnected = false;

void wifiEventHandler(WiFiEvent_t event);
void connectToWiFi(const char *ssid, const char *pwd);

void setup()
{
  // Init lcd, serial, but not the sd card
  M5.begin(true, false, true);

  connectToWiFi(networkName, networkPswd);

  uiSetup();
}

void loop()
{
  uiLoop();
}

void connectToWiFi(const char *ssid, const char *pwd)
{
  M5.Lcd.print("Connecting to ");
  M5.Lcd.println(String(ssid) + "\n");

  // Delete old config
  WiFi.disconnect(true);

  WiFi.onEvent(wifiEventHandler);

  WiFi.begin(ssid, pwd);

  M5.Lcd.println("Waiting for connection...");
}

void wifiEventHandler(WiFiEvent_t event)
{
  switch (event)
  {
  case SYSTEM_EVENT_STA_GOT_IP:
    M5.Lcd.println(" WiFi connected!");

    setupUdp();

    uiInit();

    isWifiConnected = true;
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
    M5.Lcd.println(" WiFi connection lost");

    isWifiConnected = false;
    break;
  default:
    // Other events; Do nothing
    break;
  }
}
