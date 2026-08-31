#ifndef GET_INPUT_H
#define GET_INPUT_H

#include <Arduino.h>
#include "defines.h"
#include "global_variables.h"
#include "clockInterface.h"

bool getTimeInput(struct hour_t* time);
bool getDateInput(struct dateText_t* date);

#endif