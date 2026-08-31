#include "userInput.h"

// Narysuj pasek powrót/zatwierdź
void drawOptionBar()
{
  const uint8_t charHeight = TFT_TEXT_HEIGHT * textSizeMedium;
  tft.setTextSize(textSizeMedium);
  tft.fillRect(0, TFT_HEIGHT - charHeight, TFT_WIDTH, charHeight, KOLOR_MENU);
  tft.setCursor(0, TFT_HEIGHT - charHeight + 2);
  tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
  tft.print("Powrot");
  tft.setTextColor(KOLOR_OPCJI);
  tft.print("  Zatwierdz");
}

// Get Time
void drawOption_getTimeInput(int i)
{
  const uint8_t charH = TFT_TEXT_HEIGHT * textSizeMedium;
  tft.fillRect(0, TFT_HEIGHT - charH, TFT_WIDTH, charH, KOLOR_MENU);
  tft.setTextSize(textSizeMedium);
  tft.setCursor(0, TFT_HEIGHT - charH + 2);
  if (i) {
    tft.setTextColor(KOLOR_OPCJI);
    tft.print("Powrot");
    tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
    tft.print("  Zatwierdz");
  } else {
    tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
    tft.print("Powrot");
    tft.setTextColor(KOLOR_OPCJI);
    tft.print("  Zatwierdz");
  }
  return;
}

bool getTimeInput(struct hour_t* time)
{
  uint8_t textWidth = 8 * 6 * textSizeLarge;
  uint8_t textHeight = 8 * textSizeLarge;
  uint8_t largeCharWidth = 6 * textSizeLarge;
  uint8_t highlightWidth = 2 * 6 * textSizeLarge;
  uint8_t highlightHeight = 6;
  uint8_t x = (TFT_WIDTH - textWidth) / 2;
  uint8_t y = (TFT_HEIGHT - textHeight) / 2;
  bool stayHere = true;
  uint8_t optionIndex = 0; // 0 - powrót, 1 - zatwierdz
  uint8_t modifiedIndex = 0; // 0 - godzina, 1 - minuta, 2 - sekunda
  bool a1, a2;
  a1 = false;
  uint8_t hour = RTC.getHour(a1, a2);
  uint8_t minute = RTC.getMinute();
  uint8_t second = 0;

  // Wyświetl godzinę kliknięcia
  tft.fillScreen(KOLOR_TLA);
  tft.setTextSize(textSizeLarge);
  tft.setTextColor(KOLOR_ZNAKOW);
  tft_printFullHour(x, y, hour, minute, second);

  // Wyświetl Pasek z dwoma opcjami u dołu
  drawOptionBar();

  tft.fillRect(x, y+highlightHeight+textHeight, 2*largeCharWidth, highlightHeight, KOLOR_PODKRESLENIA); // Podkreślenie godziny (startowa wartość do zmiany)

  while (stayHere)
  {
    // Enkoder lewy - zmiana wybranej wartości czasu
    result_EncL = EncL.process();
    if (result_EncL == DIR_CW) // zwiększ
    {
      switch (modifiedIndex)
      {
        case 0: // godzina
          if (hour < 23)
            ++hour;
          else
            hour = 0;
          break;
        case 1: // minuta
          if (minute < 59)
            ++minute;
          else
            minute = 0;
          break;
        case 2: // sekunda
          if (second < 59)
            ++second;
          else
            second = 0;
          break;
      }
      tft.fillRect(x, y, textWidth, textHeight, KOLOR_TLA);
      tft.setTextSize(textSizeLarge);
      tft.setTextColor(KOLOR_ZNAKOW);
      tft_printFullHour(x, y, hour, minute, second);
    } 
    else if (result_EncL == DIR_CCW) // zmniejsz
    {
      switch (modifiedIndex)
      {
        case 0: // godzina
          if (hour > 0)
            --hour;
          else
            hour = 23;
          break;
        case 1: // minuta
          if (minute > 0)
            --minute;
          else
            minute = 59;
          break;
        case 2: // sekunda
          if (second > 0)
            --second;
          else
            second = 59;
          break;
      }
      tft.fillRect(x, y, textWidth, textHeight, KOLOR_TLA);
      tft.setTextSize(textSizeLarge);
      tft.setTextColor(KOLOR_ZNAKOW);
      tft_printFullHour(x, y, hour, minute, second);
    }
    // Toggle zmienianą wartość (lewy enkoder - przycisk)
    if (digitalRead(IO_encoderLkey) == LOW)
    {
      switch (modifiedIndex)
      {
        case 0: // godzina
          modifiedIndex = 1;
          tft.fillRect(x, y+highlightHeight+textHeight, 8*largeCharWidth, highlightHeight, KOLOR_TLA); // Wyczyszczenie starego podkreślenia
          tft.fillRect(x + 3*largeCharWidth, y+highlightHeight+textHeight, 2*largeCharWidth, highlightHeight, KOLOR_PODKRESLENIA); // Podkreślenie wybranego elementu
          break;
        case 1: // minuta
          modifiedIndex = 2;
          tft.fillRect(x, y+highlightHeight+textHeight, 8*largeCharWidth, highlightHeight, KOLOR_TLA); // Wyczyszczenie starego podkreślenia
          tft.fillRect(x + 6*largeCharWidth, y+highlightHeight+textHeight, 2*largeCharWidth, highlightHeight, KOLOR_PODKRESLENIA);
          break;
        case 2: // sekunda
          modifiedIndex = 0;
          tft.fillRect(x, y+highlightHeight+textHeight, 8*largeCharWidth, highlightHeight, KOLOR_TLA); // Wyczyszczenie starego podkreślenia
          tft.fillRect(x, y+highlightHeight+textHeight, 2*largeCharWidth, highlightHeight, KOLOR_PODKRESLENIA);
          break;
      }
      while (digitalRead(IO_encoderLkey) == LOW) {}
      delay(pressDelayTime);
    }

    // Obrót prawym enkoderem (Wybór opcji)
    result_EncR = EncR.process();
    if (result_EncR == DIR_CW) // w prawo
    {
      if (!optionIndex)
      {
        optionIndex = 1;
        drawOption_getTimeInput(optionIndex);
      }
    } 
    else if (result_EncR == DIR_CCW)  // w lewo
    {
      if (optionIndex)
      {
        optionIndex = 0;
        drawOption_getTimeInput(optionIndex);
      }
    }
    // Obsługa opcji (prawy enkoder - przycisk)
    if (digitalRead(IO_encoderRkey) == LOW)
    {
      stayHere = false;
      while (digitalRead(IO_encoderRkey) == LOW) {}
      delay(pressDelayTime);
      if (optionIndex == 0) // Anuluj ustawianie daty
        return true;
      else
      {
        time->h = hour;
        time->m = minute;
        time->s = second;
        return false;
      }
    }
  }
  return false;
}

void changeDay(uint8_t& day, uint8_t month, uint8_t year, bool isAdding)
{
  uint8_t dayLimit = 31; // <-- this is essentialy default: for 31 day months
  switch(month)
  {
    case 4: case 6: case 9: case 11:  // 30 day months
      dayLimit = 30;
      break;
    case 2:   // Februarius - 28 or 29 days on leap years
      if (year % 4 == 0) // Leap year
        dayLimit = 29;
      else    // Normal year
        dayLimit = 28;
      break;
  }

  if (isAdding)
  {
    if (day < dayLimit)
      ++day;
    else
      day = 1;
  }
  else
  {
    if (day > 1)
      --day;
    else
      day = dayLimit;
  }
}

void verifyMaxDayForMonth(uint8_t& day, uint8_t month, uint8_t year)
{
  uint8_t dayLimit = 31; // <-- this is essentialy default: for 31 day months
  switch(month)
  {
    case 4: case 6: case 9: case 11:  // 30 day months
      dayLimit = 30;
      break;
    case 2:   // Februarius - 28 or 29 days on leap years
      if (year % 4 == 0) // Leap year
        dayLimit = 29;
      else    // Normal year
        dayLimit = 28;
      break;
  }

  if (day > dayLimit)
    day = dayLimit;
}

// Get Date
bool getDateInput(struct dateText_t* date)
{
  getDateIntoStruct(*date);
  // Date (textSizeBig)
  const uint8_t x = (TFT_WIDTH - (10 * TFT_TEXT_WIDTH * textSizeBig)) / 2;
  const uint8_t DATE_y = TFT_HEIGHT - 4 * TFT_TEXT_HEIGHT * textSizeBig; // Tak trochę nad barem
  const uint8_t DATE_highlightWidth = 2 * TFT_TEXT_WIDTH * textSizeBig;
  const uint8_t DATE_highlightHeight = 5;
  // Day of Week (textSizeLarge)
  const uint8_t DOW_y = DATE_y - TFT_TEXT_HEIGHT * textSizeMedium - (TFT_TEXT_HEIGHT * textSizeBig);
  uint8_t DOW_highlightWidth = date->dayName.length() * TFT_TEXT_WIDTH * textSizeLarge;
  const uint8_t DOW_highlightHeight = 6;
  // Zmienne funkcjonalne
  bool stayHere = true;
  uint8_t optionIndex = 0; // 0 - powrót, 1 - zatwierdz
  uint8_t modifiedIndex = 0; // 0 - DoW, 1 - day, 2 - month, 3 - year
  
  // Wyświetl bieżącą date
  tft.fillScreen(KOLOR_TLA);
  tft_printDoW(x, DOW_y, date->dayName, textSizeLarge);
  tft_printDate(x, DATE_y, date, textSizeBig);

  // Wyświetl Pasek z dwoma opcjami u dołu
  drawOptionBar();
  // Podkreślenie DoW (startowa wartość do zmiany)
  tft.fillRect(x, DOW_y+1+(TFT_TEXT_HEIGHT * textSizeLarge), DOW_highlightWidth, DOW_highlightHeight, KOLOR_PODKRESLENIA);

  while (stayHere)
  {
    // Enkoder lewy - zmiana wybranej wartości daty
    result_EncL = EncL.process();
    if (result_EncL == DIR_CW) // zwiększ
    {
      switch (modifiedIndex)
      {
        case 0: // DoW (1-7) 1==niedziela
          if (date->DoW < 7)
            ++date->DoW;
          else
            date->DoW = 1;

          date->dayName = getDayName(date->DoW);
          DOW_highlightWidth = date->dayName.length() * TFT_TEXT_WIDTH * textSizeLarge;
          tft.fillRect(x, DOW_y+1+(TFT_TEXT_HEIGHT * textSizeLarge), TFT_TEXT_WIDTH * textSizeLarge * 10, DOW_highlightHeight, KOLOR_TLA);
          tft.fillRect(x, DOW_y+1+(TFT_TEXT_HEIGHT * textSizeLarge), DOW_highlightWidth, DOW_highlightHeight, KOLOR_PODKRESLENIA);
          break;
        case 1: // day
          changeDay(date->day, date->month, date->year, true);
          break;
        case 2: // month (1-12)
          if (date->month < 12)
            ++date->month;
          else
            date->month = 1;
          
          verifyMaxDayForMonth(date->day, date->month, date->year);
          break;
        case 3: // year (00-99)
          if (date->year < 99)
            ++date->year;
          else
            date->year = 0;
          break;
      }
      if (modifiedIndex)  // Zmiana daty
      {
        tft.fillRect(x, DATE_y, 10 * TFT_TEXT_WIDTH * textSizeBig, TFT_TEXT_HEIGHT * textSizeBig, KOLOR_TLA);
        tft_printDate(x, DATE_y, date, textSizeBig);
      }
      else                // Zmiana DoW
      {
        tft.fillRect(x, DOW_y, 10 * TFT_TEXT_WIDTH * textSizeLarge, TFT_TEXT_HEIGHT * textSizeLarge, KOLOR_TLA);
        tft_printDoW(x, DOW_y, date->dayName, textSizeLarge);
      }
    } 
    else if (result_EncL == DIR_CCW) // zmniejsz
    {
      switch (modifiedIndex)
      {
        case 0: // DoW (1-7) 1==niedziela
          if (date->DoW > 1)
            --date->DoW;
          else
            date->DoW = 7;
          
          date->dayName = getDayName(date->DoW);
          DOW_highlightWidth = date->dayName.length() * TFT_TEXT_WIDTH * textSizeLarge;
          tft.fillRect(x, DOW_y+1+(TFT_TEXT_HEIGHT * textSizeLarge), TFT_TEXT_WIDTH * textSizeLarge * 10, DOW_highlightHeight, KOLOR_TLA);
          tft.fillRect(x, DOW_y+1+(TFT_TEXT_HEIGHT * textSizeLarge), DOW_highlightWidth, DOW_highlightHeight, KOLOR_PODKRESLENIA);
          break;
        case 1: // day (1-28,29,30,31)
          changeDay(date->day, date->month, date->year, false);
          break;
        case 2: // month (1-12)
          if (date->month > 1)
            --date->month;
          else
            date->month = 12;
          
          verifyMaxDayForMonth(date->day, date->month, date->year);
          break;
        case 3: // year (00-99)
          if (date->year > 0)
            --date->year;
          else
            date->year = 99;
          break;
      }
      if (modifiedIndex)  // Zmiana daty
      {
        tft.fillRect(x, DATE_y, 10 * TFT_TEXT_WIDTH * textSizeBig, TFT_TEXT_HEIGHT * textSizeBig, KOLOR_TLA);
        tft_printDate(x, DATE_y, date, textSizeBig);
      }
      else                // Zmiana DoW
      {
        tft.fillRect(x, DOW_y, 10 * TFT_TEXT_WIDTH * textSizeLarge, TFT_TEXT_HEIGHT * textSizeLarge, KOLOR_TLA);
        tft_printDoW(x, DOW_y, date->dayName, textSizeLarge);
      }
    }
    // Toggle zmienianą wartość (lewy enkoder - przycisk)
    if (digitalRead(IO_encoderLkey) == LOW)
    {
      // Wyczyszczenie podkreśleń
      tft.fillRect(x, DOW_y+1+(TFT_TEXT_HEIGHT * textSizeLarge), TFT_TEXT_WIDTH * textSizeLarge * 12, DOW_highlightHeight, KOLOR_TLA);  // DoW
      tft.fillRect(x, DATE_y+1+(TFT_TEXT_HEIGHT * textSizeBig), TFT_TEXT_WIDTH * textSizeBig * 10, DATE_highlightHeight, KOLOR_TLA);    // DATE
      switch (modifiedIndex)
      {
        case 0: // DoW -> Day
          modifiedIndex = 1;
          tft.fillRect(x, DATE_y+1+(TFT_TEXT_HEIGHT * textSizeBig), DATE_highlightWidth, DATE_highlightHeight, KOLOR_PODKRESLENIA);
          break;
        case 1: // Day -> Month
          modifiedIndex = 2;
          tft.fillRect(x + (3 * TFT_TEXT_WIDTH * textSizeBig), DATE_y+1+(TFT_TEXT_HEIGHT * textSizeBig), DATE_highlightWidth, DATE_highlightHeight, KOLOR_PODKRESLENIA);
          break;
        case 2: // Month -> Year
          modifiedIndex = 3;
          tft.fillRect(x + (6 * TFT_TEXT_WIDTH * textSizeBig), DATE_y+1+(TFT_TEXT_HEIGHT * textSizeBig), DATE_highlightWidth*2, DATE_highlightHeight, KOLOR_PODKRESLENIA);
          break;
        case 3: // Year -> DoW
          modifiedIndex = 0;
          tft.fillRect(x, DOW_y+1+(TFT_TEXT_HEIGHT * textSizeLarge), DOW_highlightWidth, DOW_highlightHeight, KOLOR_PODKRESLENIA);
          break;
      }
      while (digitalRead(IO_encoderLkey) == LOW) {}
      delay(pressDelayTime);
    }

    // Obrót prawym enkoderem (Wybór opcji)
    result_EncR = EncR.process();
    if (result_EncR == DIR_CW) // w prawo
    {
      if (!optionIndex)
      {
        optionIndex = 1;
        drawOption_getTimeInput(optionIndex);
      }
    } 
    else if (result_EncR == DIR_CCW)  // w lewo
    {
      if (optionIndex)
      {
        optionIndex = 0;
        drawOption_getTimeInput(optionIndex);
      }
    }
    // Obsługa opcji (prawy enkoder - przycisk)
    if (digitalRead(IO_encoderRkey) == LOW)
    {
      stayHere = false;
      while (digitalRead(IO_encoderRkey) == LOW) {}
      delay(pressDelayTime);
      if (optionIndex == 0) // Anuluj ustawianie daty
        return true;
      else
        return false;
    }
  }
  return false;
}