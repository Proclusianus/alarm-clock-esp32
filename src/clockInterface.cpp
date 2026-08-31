#include "clockInterface.h"

// ****************** DATA OPERATIONS ****************** //

String getDayName(uint8_t DoW)
{
  switch(DoW)
  {
    case 1:
      return String("Niedz.");
    case 2:
      return String("Pon.");
    case 3:
      return String("Wtorek");
    case 4:
      return String("Sroda");
    case 5:
      return String("Czwartek");
    case 6:
      return String("Piatek");
    case 7:
      return String("Sobota");
    default:
      return String("???");
  }
}

void getDateIntoStruct(struct dateText_t &d)
{
  d.day = RTC.getDate();
  bool CenturyBit;
  d.month = RTC.getMonth(CenturyBit); // Potrzebne tylko jako argument, do roku 2100 to nie musi działać
  d.year = RTC.getYear();
  d.DoW = RTC.getDoW();
  d.dayName = getDayName(d.DoW);
}

void DS3231_setTime(const hour_t& hour, const dateText_t& date)
{
  RTC.setSecond(hour.s);
  RTC.setMinute(hour.m);
  RTC.setHour(hour.h);
  RTC.setDoW(date.DoW);
  RTC.setDate(date.day);
  RTC.setMonth(date.month);
  RTC.setYear(date.year);
}

// ****************** PRINT ****************** //

void tft_displayMenuTime()
{
  // Zmienne do godziny
  const uint8_t textWidth = 8 * TFT_TEXT_WIDTH * textSizeLarge; // podstawowa czcionka ma wymiary 6x8, mnożona razy textSize; 8 - długość stringa godziny
  const uint8_t textHeight = TFT_TEXT_HEIGHT * textSizeLarge;
  const uint8_t x = (TFT_WIDTH - textWidth) / 2;
  const uint8_t y = (TFT_HEIGHT - textHeight) / 2;
  // Zmienne do daty
  const uint8_t dateWidth = 10 * TFT_TEXT_WIDTH * textSizeBig;                          // Format dd.mm.yyyy <== 10 znaków
  const uint8_t dayAndDateHeight = TFT_TEXT_HEIGHT * textSizeBig;
  const uint8_t yDayName = y + textHeight + 8 * 3;
  const uint8_t yDate = TFT_HEIGHT - dayAndDateHeight - 4; //yDayName + dayAndDateHeight + 8;
  // Zmienna z datą
  struct dateText_t date;
  getDateIntoStruct(date);

  // Wyczyść poprzednią godzinę
  tft.fillRect(x, y, textWidth, textHeight, KOLOR_TLA);

  // Wyczyść poprzednią nazwę dnia; zawsze po 12 znaków, czyli dla najdłużej możliwości
  tft.fillRect(0, yDayName, 12 * TFT_TEXT_WIDTH * textSizeBig, dayAndDateHeight, KOLOR_TLA);

  // Wyczyść poprzednią datę; 10 znaków długości
  tft.fillRect(0, yDate, dateWidth, dayAndDateHeight, KOLOR_TLA);

  // Wypisz nową godzine
  tft.setTextSize(textSizeLarge);
  tft.setTextColor(KOLOR_ZNAKOW);
  tft_printFullHour(x, y);

  // Wypisz nową datę
  tft_printDoW(0, yDayName, date.dayName, textSizeBig);
  tft_printDate(0, yDate, &date, textSizeBig);
}

// Wszystkie funkcje poniżej nie czyszczą miejsca w którym printują.

// z vectora
void tft_printFullHour(int x, int y, int i)
{
  tft.setCursor(x, y);
  uint8_t hour = alarmy[i].h;
  String hour_str = String(hour);
  uint8_t minute = alarmy[i].m;
  String minute_str = String(minute);
  uint8_t second = alarmy[i].s;
  String second_str = String(second);
  if (hour < 10)
    hour_str = '0' + String(hour);
  if (minute < 10)
    minute_str = '0' + String(minute);
  if (second < 10)
    second_str = '0' + String(second);
  String alarmText = hour_str + ":" + minute_str + ":" + second_str;
  tft.print(alarmText);
}

// z RTC
void tft_printFullHour(int x, int y)
{
  tft.setCursor(x, y);
  bool a1, a2;
  a1 = false;
  uint8_t hour = RTC.getHour(a1, a2); // wymaga zmiennych, a1=false - tryb 24-godzinny a2 - czy AM czy PM - nieużywane
  String hour_str = String(hour);
  uint8_t minute = RTC.getMinute();
  String minute_str = String(minute);
  uint8_t second = RTC.getSecond();
  String second_str = String(second);
  if (hour < 10)
    hour_str = '0' + String(hour);
  if (minute < 10)
    minute_str = '0' + String(minute);
  if (second < 10)
    second_str = '0' + String(second);

  String text = hour_str + ":" + minute_str + ":" + second_str;
  tft.print(text);
}

void tft_printFullHour(int x, int y, uint8_t hour, uint8_t minute, uint8_t second)
{
  tft.setCursor(x, y);
  String hour_str = String(hour);
  String minute_str = String(minute);
  String second_str = String(second);
  if (hour < 10)
    hour_str = '0' + String(hour);
  if (minute < 10)
    minute_str = '0' + String(minute);
  if (second < 10)
    second_str = '0' + String(second);

  String text = hour_str + ":" + minute_str + ":" + second_str;
  tft.print(text);
}

void tft_printDate(uint8_t x, uint8_t y, struct dateText_t* date, uint8_t textSize)
{
  // Wypisz nową datę
  tft.setTextSize(textSize);
  tft.setTextColor(KOLOR_ZNAKOW);
  tft.setCursor(x, y);
  String day_str = (date->day < 10) ? ('0' + String(date->day)) : String(date->day);
  String month_str = (date->month < 10) ? ('0' + String(date->month)) : String(date->month);
  String year_str = (date->year < 10) ? ('0' + String(date->year)) : String(date->year);
  String str = day_str + '.' + month_str + ".20" + year_str;
  tft.print(str);
}

void tft_printDoW(uint8_t x, uint8_t y, String& dayName, uint8_t textSize)
{
  // Wypisz nowy dzień tygodnia
  tft.setTextSize(textSize);
  tft.setTextColor(KOLOR_ZNAKOW);
  tft.setCursor(x, y);
  tft.print(dayName);
}