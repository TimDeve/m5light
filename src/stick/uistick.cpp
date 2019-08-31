#include <M5StickC.h>

void uiSetup()
{
  M5.begin();
  auto ninetyDegreeLeft = 1;
  M5.Lcd.setRotation(ninetyDegreeLeft);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK, WHITE);
};

void uiInit()
{
  M5.Lcd.println("\nInitialised.");
};

void uiLoop(){};
