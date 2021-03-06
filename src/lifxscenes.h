#pragma once
#include "lifxmessage.h"
#include "lifx.h"

namespace lifxscenes
{

typedef struct scene_t
{
    const char *name;
    uint16_t brightness;
    uint16_t kelvin;
} scene_t;

const scene_t chill = {
    .name = "Chill",
    .brightness = 9830U,
    .kelvin = 2000U,
};

const scene_t daylight = {
    .name = "Daylight",
    .brightness = 0xFFFF,
    .kelvin = 5000U,
};

const scene_t warmBright = {
    .name = "Warm bright",
    .brightness = 0xFFFF,
    .kelvin = 2700U,
};

const scene_t computer = {
    .name = "Computer",
    .brightness = 19005U,
    .kelvin = 2700U,
};

const scene_t reading = {
    .name = "Reading",
    .brightness = 32767U,
    .kelvin = 2000U,
};

const scene_t dark = {
    .name = "Dark",
    .brightness = 0x0001,
    .kelvin = 2500U,
};

const scene_t darkRed = {
    .name = "Dark Red",
    .brightness = 0x0001,
    .kelvin = 1500U,
};

const scene_t scenes[] = {chill, daylight, warmBright, computer, reading, dark, darkRed};

const size_t lengthOfScenes = sizeof(lifxscenes::scenes) / sizeof(lifxscenes::scene_t);

} // namespace lifxscenes
