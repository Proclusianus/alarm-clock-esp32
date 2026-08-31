#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <Arduino.h>
#include "Wire.h"
#include "time.h"
#include <DS3231.h>           // Potrzebny Wire.h i time.h
#include "Adafruit_GFX.h"     // Do LCD
#include "Adafruit_ILI9341.h" // Do LCD
#include <Rotary.h>           // Do Enkoderów
#include "Audio.h"            // Do dźwięku
#include "defines.h"
#include <vector>

extern DS3231 RTC; // 0x68 
extern Adafruit_ILI9341 tft;
extern Rotary EncL;
extern unsigned char result_EncL;
extern Rotary EncR;
extern unsigned char result_EncR;
extern Audio audio;
extern uint8_t cur_volume;
extern std::vector<alarmDate> alarmy;
extern bool returnToAlarmsMenu;

#endif
