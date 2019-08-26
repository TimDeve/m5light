#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include <secrets.h>
#include <lifxmessage.h>

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

void wifiEventHandler(WiFiEvent_t event);
void connectToWiFi(const char *ssid, const char *pwd);
void screenTimeout();
void turnScreenOff();
void turnScreenOn();
SetPowerMessage createTurnOffMessage();
SetPowerMessage createTurnOnMessage();
void printHexToSerial(uint8_t *arr, uint8_t size);
SetColorMessage createSetColorMessage();

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
      SetPowerMessage m = createTurnOnMessage();
      uint8_t *byteMessage = reinterpret_cast<uint8_t *>(&m);
      printHexToSerial(byteMessage, m.header.size);

      udp.beginPacket(udpAddress, udpPort);
      udp.write(byteMessage, m.header.size);
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
      SetPowerMessage m = createTurnOffMessage();
      uint8_t *byteMessage = reinterpret_cast<uint8_t *>(&m);
      printHexToSerial(byteMessage, m.header.size);

      udp.beginPacket(udpAddress, udpPort);
      udp.write(byteMessage, m.header.size);
      udp.endPacket();
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
      SetColorMessage m = createSetColorMessage();
      uint8_t *byteMessage = reinterpret_cast<uint8_t *>(&m);
      printHexToSerial(byteMessage, m.header.size);

      udp.beginPacket(udpAddress, udpPort);
      udp.write(byteMessage, m.header.size);
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

Header createHeader(uint16_t type)
{
  Header h = {0};
  h.protocol = 1024U;
  h.tagged = 1U;
  h.addressable = 1U;
  h.type = type;

  return h;
}

SetPowerMessage createSetPowerMessage()
{
  Header h = createHeader(117U);

  SetPowerPayload p = {0};
  p.duration = 256U;

  SetPowerMessage m = {
      .header = h,
      .payload = p,
  };

  m.header.size = sizeof(m);

  return m;
}

SetPowerMessage createTurnOffMessage()
{
  SetPowerMessage m = createSetPowerMessage();
  m.payload.level = 0x0000;
  return m;
}

SetPowerMessage createTurnOnMessage()
{
  SetPowerMessage m = createSetPowerMessage();
  m.payload.level = 0xFFFF;
  return m;
}

SetColorMessage createSetColorMessage()
{
  Header h = createHeader(102U);

  HSBK color = {
      .hue = 0U,
      .saturation = 0U,
      .brightness = 0xFFFF,
      .kelvin = 0xFFFF,
  };

  SetColorPayload p = {0};
  p.duration = 256U;
  p.color = color;

  SetColorMessage m = {
      .header = h,
      .payload = p,
  };

  m.header.size = sizeof(m);

  return m;
}

void printHexToSerial(uint8_t *arr, uint8_t size)
{
  for (int i = 0; i < size; i++)
  {
    if (i > 0)
      Serial.printf(":");

    Serial.printf("%02X", arr[i]);
  }
  Serial.printf("\n");
}