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

void drawSceneSelector(int32_t yPosition)
{
  M5.Lcd.fillRect(40, yPosition + 38, 240, 2, TFT_DARKGREY);
  M5.Lcd.fillTriangle(15, yPosition + 15, 30, yPosition + 7, 30, yPosition + 23, TFT_DARKGREY);
  M5.Lcd.fillTriangle(305, yPosition + 15, 290, yPosition + 7, 290, yPosition + 23, TFT_DARKGREY);
  M5.Lcd.setTextColor(TFT_DARKGREY);
  M5.Lcd.drawCentreString(lifxscenes::scenes[currentlySelectedScene].name, 160, yPosition, 2);
}

void drawBottomMenu(int32_t yPosition)
{
  M5.Lcd.setTextColor(TFT_DARKGREY);
  M5.Lcd.drawCentreString("On", 65, yPosition, 2);
  M5.Lcd.drawCentreString("Off", 160, yPosition, 2);
  M5.Lcd.drawCentreString("Switch", 255, yPosition, 2);
}

void render()
{
  M5.Lcd.clear(TFT_WHITE);

  int32_t sceneSelectorYPosition = 120;
  drawSceneSelector(sceneSelectorYPosition);

  int32_t bottomMenuYPosition = 200;
  drawBottomMenu(bottomMenuYPosition);

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

      if (currentlySelectedScene == lifxscenes::sizeofScenes - 1)
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
  // Init lcd, serial, but not the sd card
  M5.begin(true, false, true);

  M5.Lcd.clear(TFT_WHITE);
  M5.Lcd.setTextColor(TFT_DARKGREY);
  M5.Lcd.setTextSize(2);
}
