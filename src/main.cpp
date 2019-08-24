#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include <secrets.h>

const char *networkName = NETWORK_NAME;
const char *networkPswd = NETWORK_PASS;

const char *udpAddress = "10.0.0.255";
const int udpPort = 56700;

boolean isWifiConnected = false;
boolean screenOn = true;
boolean tickerAttached = false;
uint tickSinceScreenWakeUp = 0;

WiFiUDP udp;
Ticker screenTicker;

const unsigned char turnOnMessage[] = {0x28, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x75, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x01, 0x00, 0x00};
const unsigned char turnOffMessage[] = {0x28, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

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
      udp.beginPacket(udpAddress, udpPort);
      udp.write(turnOnMessage, sizeof(turnOnMessage));
      udp.endPacket();
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
      udp.beginPacket(udpAddress, udpPort);
      udp.write(turnOffMessage, sizeof(turnOffMessage));
      udp.endPacket();
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

    udp.begin(WiFi.localIP(), udpPort);

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
