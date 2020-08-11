#include "M5Atom.h"

#include "lifxscenes.h"

#define COLOR_BACKGROUND 0x0000 // Black but looks grey
#define COLOR_FOREGROUND 0x2104 // Light grey

boolean lightOn = false;
boolean uiReady = false;
boolean shouldRerender = true;
size_t currentlySelectedScene = 0;


void render()
{
}

void uiSetup()
{
};

void uiInit()
{
    uiReady = true;
};

void uiLoop()
{
};
