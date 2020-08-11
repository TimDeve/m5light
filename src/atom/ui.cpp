#include <M5Atom.h>
#include <Ticker.h>

#include "lifxscenes.h"

#define GREEN CRGB::White
#define RED CRGB::Green
#define BLUE CRGB::Blue

boolean screenOn = true;
boolean tickerAttached = false;
boolean shouldRerender = true;
boolean uiReady = false;
boolean isLightOn = true;
uint tickSinceScreenWakeUp = 0;
Ticker screenTicker;

void fillDisplay(CRGB color) {
    for (int i = 0; i < 64;i++) {
        M5.dis.drawpix(i, color);
    }
}

void turnScreenOff()
{
    screenOn = false;

    M5.dis.setBrightness(0);
}

void turnScreenOn()
{
    tickSinceScreenWakeUp = 0;

    if (screenOn == false)
    {
        screenOn = true;

        M5.dis.setBrightness(10);
    }
}

void timeout()
{
    if (tickSinceScreenWakeUp > 5 && screenOn)
    {
        turnScreenOff();

        shouldRerender = true;
    }
    else if (screenOn)
    {
        tickSinceScreenWakeUp += 1;
    }
}

void render()
{
    shouldRerender = false;

    M5.dis.clear();

    if (uiReady) {
        fillDisplay(GREEN);
    }
    else {
        fillDisplay(RED);
    }
}

void uiSetup()
{
    M5.begin(true, false, true);
    M5.dis.setBrightness(10);
}

void uiInit()
{
    uiReady = true;
    shouldRerender = true;

    if (!tickerAttached)
    {
        screenTicker.attach(1, timeout);
        tickerAttached = true;
    }
};

void toggleLight() {
    if (isLightOn) {
        isLightOn = false;
        sendTurnOffMessage();
    }
    else {
        isLightOn =true;
        sendTurnOnMessage();
    }
}

void uiLoop()
{
    M5.update();

    if (M5.Btn.wasReleased())
    {
        toggleLight();
        turnScreenOn();
    }

    if (shouldRerender)
    {
        render();
    }
    delay(50);
};
