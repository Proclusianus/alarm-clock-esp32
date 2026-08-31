#ifndef SETUP_INIT_H
#define SETUP_INIT_H

#include <Arduino.h>
#include "defines.h"
#include "global_variables.h"
#include "wifiHandling.h"

void setup_pins();
void setup_clockTime();
void setup_tft();
void setup_audio();
void setup_song();
void setup_Devices();

#endif