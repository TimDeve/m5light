#include <M5StickC.h>

#include "lifxscenes.h"

#define COLOR_BACKGROUND 0x0000 // Black but looks grey
#define COLOR_FOREGROUND 0x2104 // Light grey

boolean lightOn = false;
boolean uiReady = false;
boolean shouldRerender = true;
size_t currentlySelectedScene = 0;

void setCurrentlySelectedScene()
{
  sendTurnOnMessage();

  auto currentScene = lifxscenes::scenes[currentlySelectedScene];
  sendSetColorMessage(currentScene.brightness, currentScene.kelvin);
}

void rotateScenes()
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

void render()
{
  M5.Lcd.fillScreen(COLOR_BACKGROUND);

  M5.Lcd.setTextColor(COLOR_FOREGROUND, COLOR_BACKGROUND);
  M5.Lcd.drawCentreString(lifxscenes::scenes[currentlySelectedScene].name, 80, 30, 4);

  shouldRerender = false;
}

void uiSetup()
{
  M5.begin();

  auto ninetyDegreeLeft = 1;
  M5.Lcd.setRotation(ninetyDegreeLeft);
  M5.Axp.ScreenBreath(10);

  M5.Lcd.fillScreen(COLOR_BACKGROUND);
  M5.Lcd.setTextColor(COLOR_FOREGROUND, COLOR_BACKGROUND);
};

void uiInit()
{
  uiReady = true;
};

void uiLoop()
{
  if (uiReady)
  {
    M5.update();

    if (M5.BtnA.wasReleased())
    {
      rotateScenes();
    }
    else if (M5.BtnA.wasReleasefor(300))
    {
      lightOn = true;
      setCurrentlySelectedScene();
    }
    else if (M5.BtnB.wasReleased())
    {
      if (lightOn)
      {
        lightOn = false;
        sendTurnOffMessage();
      }
      else
      {
        lightOn = true;
        sendTurnOnMessage();
      }
    }

    if (shouldRerender)
    {
      render();
    }
  }
};
