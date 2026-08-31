#ifndef CLOCK_INTERFACE_H
#define CLOCK_INTERFACE_H

#include <Arduino.h>
#include "defines.h"
#include "global_variables.h"

// Data operations
String getDayName(uint8_t DoW);
void getDateIntoStruct(struct dateText_t &d);
void DS3231_setTime(const hour_t& hour, const dateText_t& date);

// Prints
void tft_displayMenuTime();
void tft_printFullHour(int x, int y, int i);
void tft_printFullHour(int x, int y);
void tft_printFullHour(int x, int y, uint8_t hour, uint8_t minute, uint8_t second);
void tft_printDate(uint8_t x, uint8_t y, struct dateText_t* date, uint8_t textSize);
void tft_printDoW(uint8_t x, uint8_t y, String& dayName, uint8_t textSize);

#endif