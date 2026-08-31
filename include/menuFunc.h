#ifndef MENU_FUNC_H
#define MENU_FUNC_H

#include <Arduino.h>
#include "defines.h"
#include "global_variables.h"
#include <vector>

void menuFunction(const char* par_menuName, uint8_t par_encR_optionsAmount, const char** par_encR_optionsNames, std::vector<uint8_t>& indexesToBlock, std::vector<String>& par_items, 
    uint8_t par_itemTextSize, const char* par_noItemsMsg, bool (*par_EncL_Key_fcnptr)(uint16_t&, uint16_t&), bool (*par_EncR_Key_fcnptr)(uint8_t&, uint8_t&, bool&), uint8_t par_indexToRedirectTo=0);
void userConfirm(String displayText);

#endif