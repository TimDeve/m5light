#pragma once
#include "lifxmessage.h"
#include "lifx.h"

namespace lifxscenes
{

typedef struct scene_t
{
  char *name;
  SetColorMessage message;
} scene_t;

const scene_t chill = {
    .name = "Chill",
    .message = createSetColorMessage(9830U, 2000U),
};

const scene_t daylight = {
    .name = "Daylight",
    .message = createSetColorMessage(0xFFFF, 5000U),
};

const scene_t warmBright = {
    .name = "Warm bright",
    .message = createSetColorMessage(0xFFFF, 2700U),
};

const scene_t computer = {
    .name = "Computer",
    .message = createSetColorMessage(19005U, 2700U),
};

const scene_t reading = {
    .name = "Reading",
    .message = createSetColorMessage(32767U, 2000U),
};

const scene_t scenes[] = {chill, daylight, warmBright, computer, reading};

} // namespace lifxscenes
