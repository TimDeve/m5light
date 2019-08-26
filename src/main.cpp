#include <M5Stack.h>
#include <WiFi.h>
#include <Ticker.h>

#include "secrets.h"
#include "lifx.h"

const char *networkName = NETWORK_NAME;
const char *networkPswd = NETWORK_PASS;

boolean isWifiConnected = false;
boolean screenOn = true;
boolean tickerAttached = false;
uint tickSinceScreenWakeUp = 0;

Ticker screenTicker;

void wifiEventHandler(WiFiEvent_t event);
void connectToWiFi(const char *ssid, const char *pwd);
void screenTimeout();
void turnScreenOff();
void turnScreenOn();

void setup()
{
  // Init lcd, serial, but not the sd card
  M5.begin(true, false, true);

  M5.Lcd.clear(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextSize(2);

  connectToWiFi(networkName, networkPswd);
}

void loop()
{
  M5.update();

  if (M5.BtnA.wasReleased())
  {
    turnScreenOn();

    if (isWifiConnected)
    {
      sendTurnOnMessage();
    }
    else
    {
      M5.Lcd.println(" Wait...");
    }
  }
  else if (M5.BtnB.wasReleased())
  {
    turnScreenOn();

    if (isWifiConnected)
    {
      sendTurnOffMessage();
    }
    else
    {
      M5.Lcd.println(" Wait...");
    }
  }
  else if (M5.BtnC.wasReleased())
  {
    turnScreenOn();

    if (isWifiConnected)
    {
      sendSetColorMessage();
    }
    else
    {
      M5.Lcd.println(" Wait...");
    }
  }
}

void connectToWiFi(const char *ssid, const char *pwd)
{
  M5.Lcd.print("\n Connecting to ");
  M5.Lcd.println(String(ssid) + "\n");

  // Delete old config
  WiFi.disconnect(true);

  WiFi.onEvent(wifiEventHandler);

  WiFi.begin(ssid, pwd);

  M5.Lcd.println(" Waiting for connection...\n");
}

void wifiEventHandler(WiFiEvent_t event)
{
  switch (event)
  {
  case SYSTEM_EVENT_STA_GOT_IP:
    M5.Lcd.println(" WiFi connected!");

    setupUdp();

    if (!tickerAttached)
    {
      screenTicker.attach(1, screenTimeout);
      tickerAttached = true;
    }

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

void screenTimeout()
{
  if (tickSinceScreenWakeUp > 3 && screenOn)
  {
    turnScreenOff();
  }
  else if (screenOn)
  {
    tickSinceScreenWakeUp += 1;
  }
}

void turnScreenOff()
{
  screenOn = false;

  M5.Lcd.setBrightness(0);
}

void turnScreenOn()
{
  tickSinceScreenWakeUp = 0;

  if (screenOn == false)
  {
    screenOn = true;

    M5.Lcd.setBrightness(200);
  }
}
