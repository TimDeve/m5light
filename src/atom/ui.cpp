#include <M5Atom.h>
#include <Ticker.h>

#include "lifxscenes.h"

#define ORANGE 0xAAFF00
#define RED 0x00FF00

#define WARM 2700U

enum LastAction
{
    Toggle,
    SetBrightness
};

boolean screenOn = true;
boolean tickerAttached = false;
boolean shouldRerender = true;
boolean uiReady = false;
boolean isLightOn = true;
uint tickSinceScreenWakeUp = 0;
Ticker screenTicker;
LastAction lastAction = Toggle;

typedef struct AccelData
{
    float x;
    float y;
    float z;
} AccelData;

AccelData getAccelData()
{
    AccelData data;
    M5.IMU.getAccelData(&data.x, &data.y, &data.z);
    return data;
}

uint16_t getBrightnessFromAccel(float accel)
{
    auto val = (accel * 100 + 100) * 327.675;

    if (val > UINT16_MAX)
    {
        return UINT16_MAX;
    }
    else if (val < 0)
    {
        return 0;
    }

    return val;
}

void fillDisplay(CRGB color)
{
    for (int i = 0; i < 64; i++)
    {
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

        M5.dis.setBrightness(5);
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

    if (uiReady)
    {
        fillDisplay(ORANGE);
    }
    else
    {
        fillDisplay(RED);
    }
}

void uiSetup()
{
    M5.begin(true, false, true);
    M5.IMU.Init();
    M5.dis.setBrightness(5);
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

void toggleLight()
{
    if (isLightOn)
    {
        isLightOn = false;
        sendTurnOffMessage();
    }
    else
    {
        isLightOn = true;
        sendTurnOnMessage();
    }
}

void uiLoop()
{
    M5.update();

    if (M5.Btn.pressedFor(300))
    {
        lastAction = SetBrightness;
        auto accelData = getAccelData();
        int brightness = getBrightnessFromAccel(accelData.x);
        isLightOn = true;
        sendTurnOnMessage();
        sendSetColorMessage(brightness, WARM);
    }
    else if (M5.Btn.wasReleased())
    {
        if (lastAction == Toggle)
        {
            toggleLight();
            turnScreenOn();
        }

        lastAction = Toggle;
    }

    if (shouldRerender)
    {
        render();
    }
    delay(50);
};
