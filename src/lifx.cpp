#include <WiFi.h>
#include <WiFiUdp.h>

#include "lifx.h"
#include "secrets.h"

const char *udpAddress = BROADCAST_ADDR;
const int udpPort = 56700;

WiFiUDP udp;

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

void setupUdp()
{
  udp.begin(WiFi.localIP(), udpPort);
}

void setMacAddress(uint8_t target[8])
{
  uint8_t source[8] = BULB_MAC;
  for (int i = 0; i < 8; i++)
  {
    if (i < 6)
    {
      target[i] = source[i];
    }
    else
    {
      target[i] = 0x00;
    }
  }
}

Header createHeader(uint16_t type)
{
  Header h = {0};
  h.protocol = 1024U;
  h.tagged = 0U;
  h.addressable = 1U;
  h.type = type;
  setMacAddress(h.target);

  return h;
}

SetPowerMessage createSetPowerMessage()
{
  auto h = createHeader(117U);

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

SetColorMessage createSetColorMessage(uint16_t brightness, uint16_t kelvin)
{
  auto h = createHeader(102U);

  HSBK color = {
      .hue = 0U,
      .saturation = 0U,
      .brightness = brightness,
      .kelvin = kelvin,
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

void sendTurnOnMessage()
{
  auto m = createTurnOnMessage();
  auto *byteMessage = reinterpret_cast<uint8_t *>(&m);
  printHexToSerial(byteMessage, m.header.size);

  udp.beginPacket(udpAddress, udpPort);
  udp.write(byteMessage, m.header.size);
  udp.endPacket();
}

void sendTurnOffMessage()
{
  auto m = createTurnOffMessage();
  auto *byteMessage = reinterpret_cast<uint8_t *>(&m);
  printHexToSerial(byteMessage, m.header.size);

  udp.beginPacket(udpAddress, udpPort);
  udp.write(byteMessage, m.header.size);
  udp.endPacket();
}

void sendSetColorMessage(uint16_t brightness, uint16_t kelvin)
{
  auto message = createSetColorMessage(brightness, kelvin);
  auto *byteMessage = reinterpret_cast<uint8_t *>(&message);
  printHexToSerial(byteMessage, message.header.size);

  udp.beginPacket(udpAddress, udpPort);
  udp.write(byteMessage, message.header.size);
  udp.endPacket();
}
