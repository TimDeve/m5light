#include <M5Stack.h>
#include <Ticker.h>

#include "lifx.h"
#include "lifxscenes.h"

#define COLOR_BACKGROUND 0x18E3 // Dark grey
#define COLOR_FOREGROUND 0xAD75 // Light grey
#define COLOR_ACTIVE 0xF52A     // Dark orange

#define LCD_BRIGHTNESS 50

boolean uiReady = false;
boolean screenOn = true;
boolean tickerAttached = false;
boolean shouldRerender = true;
uint tickSinceScreenWakeUp = 0;
size_t currentlySelectedScene = 0;
Ticker screenTicker;

enum modes
{
  power,
  sceneSelector
};

modes currentlySelectedMode = power;

void toggleMode()
{
  currentlySelectedMode = currentlySelectedMode == power ? sceneSelector : power;
}

void selectSceneLeft()
{
  if (currentlySelectedScene == 0)
  {
    currentlySelectedScene = lifxscenes::lengthOfScenes - 1;
  }
  else
  {
    currentlySelectedScene--;
  }

  shouldRerender = true;
}

void selectSceneRight()
{
  if (currentlySelectedScene == lifxscenes::lengthOfScenes - 1)
  {
    currentlySelectedScene = 0;
  }
  else
  {
    currentlySelectedScene++;
  }

  shouldRerender = true;
}

void setCurrentlySelectedScene()
{
  sendTurnOnMessage();

  auto currentScene = lifxscenes::scenes[currentlySelectedScene];
  sendSetColorMessage(currentScene.brightness, currentScene.kelvin);
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

    M5.Lcd.setBrightness(LCD_BRIGHTNESS);
  }
}

void timeout()
{
  if (tickSinceScreenWakeUp > 5 && screenOn)
  {
    turnScreenOff();

    shouldRerender = true;
    currentlySelectedMode = power;
  }
  else if (screenOn)
  {
    tickSinceScreenWakeUp += 1;
  }
}

void drawPowerSymbol(int32_t yPosition)
{
  int32_t xPosition = 160;

  uint16_t foregroundColor = COLOR_FOREGROUND;
  if (currentlySelectedMode == power)
  {
    foregroundColor = COLOR_ACTIVE;
  }

  M5.Lcd.fillCircle(xPosition, yPosition, 25, foregroundColor);
  M5.Lcd.fillCircle(xPosition, yPosition, 21, COLOR_BACKGROUND);
  M5.Lcd.fillRect(xPosition - 5, yPosition - 30, 12, 20, COLOR_BACKGROUND);
  M5.Lcd.fillRect(xPosition - 1, yPosition - 30, 4, 20, foregroundColor);
}

void drawSceneSelector(int32_t yPosition)
{
  uint16_t foregroundColor = COLOR_FOREGROUND;
  if (currentlySelectedMode == sceneSelector)
  {
    foregroundColor = COLOR_ACTIVE;
  }

  M5.Lcd.fillRect(40, yPosition + 38, 240, 2, foregroundColor);
  M5.Lcd.fillTriangle(15, yPosition + 15, 30, yPosition + 7, 30, yPosition + 23, foregroundColor);
  M5.Lcd.fillTriangle(305, yPosition + 15, 290, yPosition + 7, 290, yPosition + 23, foregroundColor);
  M5.Lcd.setTextColor(foregroundColor);
  M5.Lcd.drawCentreString(lifxscenes::scenes[currentlySelectedScene].name, 160, yPosition, 2);
}

void drawBottomMenu(int32_t yPosition)
{
  M5.Lcd.setTextColor(COLOR_FOREGROUND);

  if (currentlySelectedMode == power)
  {
    M5.Lcd.drawCentreString("On", 65, yPosition, 2);
    M5.Lcd.drawCentreString("Off", 160, yPosition, 2);
  }
  else
  {
    M5.Lcd.drawCentreString("Left", 65, yPosition, 2);
    M5.Lcd.drawCentreString("Right", 160, yPosition, 2);
  }

  M5.Lcd.drawCentreString("Apply", 255, yPosition - 15, 1);
  M5.Lcd.drawCentreString("Switch", 255, yPosition, 2);
}

void render()
{
  M5.Lcd.clear(COLOR_BACKGROUND);

  int32_t powerSymbolYPosition = 50;
  drawPowerSymbol(powerSymbolYPosition);

  int32_t sceneSelectorYPosition = 108;
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

      if (currentlySelectedMode == power)
      {
        sendTurnOnMessage();
      }
      else
      {
        selectSceneLeft();
      }
    }
    else if (M5.BtnB.wasReleased())
    {
      turnScreenOn();

      if (currentlySelectedMode == power)
      {
        sendTurnOffMessage();
      }
      else
      {
        selectSceneRight();
      }
    }
    else if (M5.BtnC.wasReleased())
    {
      turnScreenOn();

      toggleMode();

      shouldRerender = true;
    }
    else if (M5.BtnC.wasReleasefor(300))
    {
      turnScreenOn();

      setCurrentlySelectedScene();
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
    screenTicker.attach(1, timeout);
    tickerAttached = true;
  }
}

void uiSetup()
{
  // Init lcd, serial, but not the sd card
  M5.begin(true, false, true);

  M5.Lcd.setBrightness(LCD_BRIGHTNESS);

  M5.Lcd.clear(COLOR_BACKGROUND);
  M5.Lcd.setTextColor(COLOR_FOREGROUND);
  M5.Lcd.setTextSize(2);
}
