#include <M5Stack.h>
#include <Ticker.h>

#include "lifx.h"
#include "lifxscenes.h"

boolean uiReady = false;
boolean screenOn = true;
boolean tickerAttached = false;
boolean shouldRerender = true;
uint tickSinceScreenWakeUp = 0;
size_t currentlySelectedScene = 0;
Ticker screenTicker;

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

void screenTimeout()
{
  if (tickSinceScreenWakeUp > 5 && screenOn)
  {
    turnScreenOff();
  }
  else if (screenOn)
  {
    tickSinceScreenWakeUp += 1;
  }
}

void render()
{
  M5.Lcd.clear(TFT_WHITE);

  M5.Lcd.fillRect(40, 98, 240, 2, TFT_DARKGREY);
  M5.Lcd.fillTriangle(15, 75, 30, 67, 30, 83, TFT_DARKGREY);
  M5.Lcd.fillTriangle(305, 75, 290, 67, 290, 83, TFT_DARKGREY);
  M5.Lcd.setTextColor(TFT_DARKGREY);
  M5.Lcd.drawCentreString(lifxscenes::scenes[currentlySelectedScene].name, 160, 60, 2);

  M5.Lcd.setTextColor(TFT_DARKGREY);
  M5.Lcd.drawCentreString("On", 65, 200, 2);
  M5.Lcd.drawCentreString("Off", 160, 200, 2);
  M5.Lcd.drawCentreString("Switch", 255, 200, 2);

  shouldRerender = false;
}

void uiLoop()
{
  if (uiReady)
  {
    M5.update();

    if (M5.BtnA.wasReleased())
    {
      turnScreenOn();

      sendTurnOnMessage();
    }
    else if (M5.BtnB.wasReleased())
    {
      turnScreenOn();

      sendTurnOffMessage();
    }
    else if (M5.BtnC.wasReleased())
    {
      turnScreenOn();

      if (currentlySelectedScene == sizeof(lifxscenes::scenes) / sizeof(lifxscenes::scene_t) - 1)
      {
        currentlySelectedScene = 0;
      }
      else
      {
        currentlySelectedScene++;
      }

      shouldRerender = true;
    }
    else if (M5.BtnC.wasReleasefor(300))
    {
      turnScreenOn();

      sendTurnOnMessage();

      auto currentScene = lifxscenes::scenes[currentlySelectedScene];
      sendSetColorMessage(currentScene.brightness, currentScene.kelvin);
    }

    if (shouldRerender)
    {
      render();
    }
  }
}

void uiInit()
{
  uiReady = true;

  if (!tickerAttached)
  {
    screenTicker.attach(1, screenTimeout);
    tickerAttached = true;
  }
}

void uiSetup()
{
  M5.Lcd.clear(TFT_WHITE);
  M5.Lcd.setTextColor(TFT_DARKGREY);
  M5.Lcd.setTextSize(2);
}