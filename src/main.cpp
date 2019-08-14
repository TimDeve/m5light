#include <M5Stack.h>

void setup()
{
  // init lcd, serial, but don't init sd card
  M5.begin(true, false, true);
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(4);
}

void loop()
{
  M5.update();

  if (M5.BtnA.wasReleased())
  {
    M5.Lcd.print('A');
  }
  else if (M5.BtnB.wasReleased())
  {
    M5.Lcd.print('B');
  }
  else if (M5.BtnC.wasReleased())
  {
    M5.Lcd.print('C');
  }
  else if (M5.BtnB.wasReleasefor(700))
  {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(0, 0);
  }
}
