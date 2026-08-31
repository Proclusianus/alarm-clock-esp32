// ****************** INCLUDES ****************** //

#include <Arduino.h>
#include "defines.h"
#include "global_variables.h"
#include "menuFunc.h"
#include "setup_init.h"
#include "userInput.h"
#include "clockInterface.h"

// ****************** FORWARD DELCARS ****************** //

void display_Alarms();

// ****************** ZMIENNE URZĄDZEŃ ****************** //
DS3231 RTC; // 0x68 
Adafruit_ILI9341 tft = Adafruit_ILI9341(IO_lcd_CS, IO_lcd_DC, IO_lcd_RST);
Rotary EncL = Rotary(IO_encoderLs1, IO_encoderLs2);
unsigned char result_EncL;
Rotary EncR = Rotary(IO_encoderRs1, IO_encoderRs2);
unsigned char result_EncR;
Audio audio;
uint8_t cur_volume = 5;
std::vector<alarmDate> alarmy;
bool returnToAlarmsMenu = false;

// ****************** MENUS' DEFINITIONS ****************** //

namespace SetTimeMenu
{
  // Function call
  // menuFunction(SetTimeMenu::menuName, SetTimeMenu::optionsAmount, &SetTimeMenu::optionString, SetTimeMenu::indexesToBlock, SetTimeMenu::items, textSizeBig, SetTimeMenu::noItemsMsg, &SetTimeMenu::EncL_Key, &SetTimeMenu::EncR_Key);

  // Zmienne dla menu ustawiania czasu
  const char* menuName = "Ustawianie Czasu";
  constexpr uint8_t optionsAmount=1;
  const char* optionString="Powrot";
  std::vector<uint8_t> indexesToBlock{};
  std::vector<String> items{String("Z internetu"), String("Recznie")};
  // textSizeBig
  const char* noItemsMsg="Brak Opcji";

  void setTimeUser()
  {
    bool doReturn = false;
    struct hour_t hour;
    doReturn = getTimeInput(&hour);
    struct dateText_t date;
    doReturn = getDateInput(&date);

    if (doReturn)
      return;
    else
    {
      DS3231_setTime(hour, date); // zmień dane
      userConfirm(String("At fourteen he runs away. He will not see again the freezing kitchenhouse in the predawn dark. The firewood, the washpots. He wanders west as far as Memphis, a solitary migrant upon that flat and pastoral landscape. Blacks in the fields, lank and stooped, their fingers spiderlike among the bolls of cotton. A shadowed agony in the garden."));
    }
  }
  
  // Tylko wyjście
  bool EncR_Key(uint8_t& optionIndex, uint8_t& maxOptionIndex, bool& stayHere)
  {
    stayHere = false;
    while (digitalRead(IO_encoderRkey) == LOW) {}
    delay(pressDelayTime);
    tft.fillScreen(KOLOR_TLA);
    return false;
  }

  bool EncL_Key(uint16_t& itemIndex, uint16_t& unused)
  {
    while (digitalRead(IO_encoderLkey) == LOW) {}
    delay(pressDelayTime);
    switch (itemIndex)
    {
      case 0:
        
        break;
      case 1:
        setTimeUser();
        break;
    }
    return true;
  }
}

namespace MainMenu 
{
  // Function call
  // menuFunction(MainMenu::mainMenuName, MainMenu::optionsAmount, &MainMenu::optionString, MainMenu::indexesToBlock, MainMenu::items, textSizeBig, MainMenu::noItemsMsg, &MainMenu::EncL_Key, &MainMenu::EncR_Key);

  // Zmienne dla menu budzika
  const char* mainMenuName="Menu Budzika";
  constexpr uint8_t optionsAmount=1;
  const char* optionString="Powrot Do Zegara";
  const char* noItemsMsg="Brak Opcji";
  std::vector<String> items{String("Alarmy"), String("Ustaw Czas"), String("Muzyka")};
  std::vector<uint8_t> indexesToBlock{};

  // Tylko jedna opcja - powrót
  bool EncR_Key(uint8_t& optionIndex, uint8_t& maxOptionIndex, bool& stayHere)
  {
    stayHere = false;
    while (digitalRead(IO_encoderRkey) == LOW) {}
    delay(pressDelayTime);
    tft.fillScreen(KOLOR_TLA);
    return false;
  }

  bool EncL_Key(uint16_t& itemIndex, uint16_t& unused)
  {
    while (digitalRead(IO_encoderLkey) == LOW) {}
    delay(pressDelayTime);
    switch (itemIndex)
    {
      case 0:
        display_Alarms();
        break;
      case 1:
        menuFunction(SetTimeMenu::menuName, SetTimeMenu::optionsAmount, &SetTimeMenu::optionString, SetTimeMenu::indexesToBlock, SetTimeMenu::items, textSizeBig, SetTimeMenu::noItemsMsg, &SetTimeMenu::EncL_Key, &SetTimeMenu::EncR_Key);
        break;
      case 2:

        break;
    }
    return true;
  }
}

// ****************** FUNKCJE MENU ****************** //

void doOption_addAlarm(int i, uint8_t h, uint8_t m, uint8_t s)
{
  while (digitalRead(IO_encoderRkey) == LOW) {}
  delay(pressDelayTime);
  tft.fillScreen(KOLOR_TLA);
  if (i) // przy dodaniu alarmu trzeba go dodać do vectora, przy i==0 nic nie zmieniam
  {
    alarmDate t1 = {h, m, s, true};
    alarmy.push_back(t1);
  }
  // returnToAlarmsMenu już ustawione przy wyjściu z funkcji displayAlarms
}

void drawOption_addAlarm(int charHeight, int i)
{
  tft.fillRect(0, TFT_HEIGHT - charHeight, TFT_WIDTH, charHeight, KOLOR_MENU);
  tft.setTextSize(textSizeMedium);
  tft.setCursor(0, TFT_HEIGHT - charHeight + 2);
  switch (i)
  {
    case 0:
      tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
      tft.print("Powrot");
      tft.setTextColor(KOLOR_OPCJI);
      tft.print("  Zatwierdz");
      return;
    case 1:
      tft.setTextColor(KOLOR_OPCJI);
      tft.print("Powrot");
      tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
      tft.print("  Zatwierdz");
      return;
  }
}

void addAlarm()
{
  int textWidth = 8 * 6 * textSizeLarge;
  int textHeight = 8 * textSizeLarge;
  int largeCharWidth = 6 * textSizeLarge;
  int highlightWidth = 2 * 6 * textSizeLarge;
  int highlightHeight = 6;
  int x = (TFT_WIDTH - textWidth) / 2;
  int y = (TFT_HEIGHT - textHeight) / 2;
  int charHeight = TFT_TEXT_HEIGHT * textSizeMedium;
  bool stayHere = true;
  int optionIndex = 0; // 0 - powrót, 1 - zatwierdz
  int modifiedIndex = 0; // 0 - godzina, 1 - minuta, 2 - sekunda
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
  tft.setTextSize(textSizeMedium);
  tft.fillRect(0, TFT_HEIGHT - charHeight, TFT_WIDTH, charHeight, KOLOR_MENU);
  tft.setCursor(0, TFT_HEIGHT - charHeight + 2);
  tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
  tft.print("Powrot");
  tft.setTextColor(KOLOR_OPCJI);
  tft.print("  Zatwierdz");
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
      optionIndex = 1;
      drawOption_addAlarm(charHeight, optionIndex);
    } 
    else if (result_EncR == DIR_CCW)  // w lewo
    {
      optionIndex = 0;
      drawOption_addAlarm(charHeight, optionIndex);
    }
    // Obsługa opcji (prawy enkoder - przycisk)
    if (digitalRead(IO_encoderRkey) == LOW)
    {
      stayHere = false;
      doOption_addAlarm(optionIndex, hour, minute, second); // może ustawić flage globalną returnToAlarmsMenu (opcje 1 i 2)
    }
  }
}

void doOption_Alarms(int i, int selectedIndex)
{
  while (digitalRead(IO_encoderRkey) == LOW) {}
  delay(pressDelayTime);
  switch (i)
  {
    case 0: // powrót do start screena
      tft.fillScreen(KOLOR_TLA);
      break;
    case 1:
      addAlarm();
      returnToAlarmsMenu = true;
      break;
    case 2:
      alarmy.erase(alarmy.begin() + selectedIndex);
      returnToAlarmsMenu = true;
      break;
  }
}

void drawOption_Alarms(int charHeight, int i)
{
  tft.fillRect(0, TFT_HEIGHT - charHeight, TFT_WIDTH, charHeight, KOLOR_MENU);
  tft.setCursor(0, TFT_HEIGHT - charHeight + 2);
  switch (i)
  {
    case 0:
      tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
      tft.print("Powrot");
      tft.setTextColor(KOLOR_OPCJI);
      tft.print(" Dodaj Usun");
      return;
    case 1:
      tft.print("Powrot");
      tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
      tft.print(" Dodaj");
      tft.setTextColor(KOLOR_OPCJI);
      tft.print(" Usun");
      return;
    case 2:
      tft.print("Powrot Dodaj" );
      tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
      tft.print(" Usun");
      tft.setTextColor(KOLOR_OPCJI);
      return;
  }
}

void drawInitial_Alarms(int dataHeight, int charWidth, int breakHeight)
{
  tft.setTextColor(KOLOR_ZNAKOW);
  if (alarmy.empty())
  {
    tft.setCursor(0, dataHeight);
    tft.print("Brak Alarmow");
  }
  else
  {
    tft.setCursor(11 * charWidth, dataHeight);  // Musić być przynajmniej jeden alarm
    tft.print("<--");                           // Ustaw wskaźnik na pierwszy alarm
    int i=0;
    for (; i < 5 && i < alarmy.size(); ++i)
    {
      tft_printFullHour(0, dataHeight + i*dataHeight, i);
      if (alarmy[i].isActive)
      {
        tft.setCursor(9 * charWidth, dataHeight + i*dataHeight);
        tft.print("X");
      }
    }
    if (alarmy.size() > i)
    {
      tft.setCursor(0, 6*dataHeight - breakHeight/2);
      tft.print("vvvvvvvv");
    }
  }
}

void drawSelected_Alarms(int dataHeight, int charWidth, int breakHeight, int charHeight, int startIndex)
{
  tft.fillRect(0, charHeight, charWidth * 10, TFT_HEIGHT-2*charHeight-1, KOLOR_TLA); // wyczyść alarmy
  tft.setTextColor(KOLOR_ZNAKOW);
  int i=startIndex; // indeks alarmu
  int j=0;          // pozycja do rysowania
  for (; j < 5; ++i, ++j)
  {
    tft_printFullHour(0, dataHeight + j*dataHeight, i);
    if (alarmy[i].isActive)
    {
      tft.setCursor(9 * charWidth, dataHeight + j*dataHeight);
      tft.print("X");
    }
  }
  if (alarmy.size() > i)
  {
    tft.setCursor(0, 6*dataHeight - breakHeight/2);
    tft.print("vvvvvvvv");
  }
}

void display_Alarms()
{
  tft.fillScreen(KOLOR_TLA); // wyczyść ekran na start
  tft.setTextSize(textSizeMedium);
  int charWidth = TFT_TEXT_WIDTH * textSizeMedium;
  int charHeight = TFT_TEXT_HEIGHT * textSizeMedium;
  int breakHeight = TFT_TEXT_HEIGHT;
  int dataHeight = charHeight + breakHeight;
  int alarmIndex = 0;
  int startAlarmIndex = 0;
  int optionIndex = 0; // 0 - powrót, 1 - dodaj, 2 - usuń
  bool stayHere = true;

  // Utworzenie menu
  tft.fillRect(0, 0, TFT_WIDTH, charHeight, KOLOR_MENU);
  tft.fillRect(0, TFT_HEIGHT - charHeight, TFT_WIDTH, charHeight, KOLOR_MENU);
  tft.setTextColor(KOLOR_OPCJI);
  tft.setCursor(0, 0);
  tft.print("Menu Alarmow");
  tft.setCursor(0, TFT_HEIGHT - charHeight + 2);
  tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
  tft.print("Powrot");
  tft.setTextColor(KOLOR_OPCJI);
  tft.print(" Dodaj Usun");

  // Wypełnienie danymi (na start)
  drawInitial_Alarms(dataHeight, charWidth, breakHeight);

  // Dynamiczne wyświetlanie danych i obsługa inputu użytkownika
  while(stayHere)
  {
    // Obrót lewym enkoderem (Wybór alarmu)
    result_EncL = EncL.process();
    if (result_EncL == DIR_CW) 
    {
      if (alarmIndex < alarmy.size() - 1)
      {
        if (alarmIndex - startAlarmIndex == 4) // załaduj kolejny alarm (przerysuj alarmy); strzałka pozostaje w tym samym miejscu
        {
          ++startAlarmIndex;
          ++alarmIndex;

          drawSelected_Alarms(dataHeight, charWidth, breakHeight, charHeight, startAlarmIndex);
        }
        else // Zmień tylko pozycję strzałki
        {
          tft.setTextColor(KOLOR_ZNAKOW);
          tft.fillRect(11 * charWidth, dataHeight + (alarmIndex - startAlarmIndex)*dataHeight, charWidth * 3, charHeight, KOLOR_TLA);
          ++alarmIndex;
          tft.setCursor(11 * charWidth, dataHeight + (alarmIndex - startAlarmIndex)*dataHeight);
          tft.print("<--");
        }
      }
    } else if (result_EncL == DIR_CCW) 
    {
      if (alarmIndex > 0)
      {
        if (alarmIndex == startAlarmIndex) // załaduj poprzednie alarmy i porusz strzałką
        {
          tft.setTextColor(KOLOR_ZNAKOW);
          --startAlarmIndex;
          tft.fillRect(11 * charWidth, dataHeight + (alarmIndex - startAlarmIndex)*dataHeight, charWidth * 3, charHeight, KOLOR_TLA);
          --alarmIndex;
          tft.setCursor(11 * charWidth, dataHeight + (alarmIndex - startAlarmIndex)*dataHeight);
          tft.print("<--");

          drawSelected_Alarms(dataHeight, charWidth, breakHeight, charHeight, startAlarmIndex);
        }
        else // Zmień tylko pozycję strzałki
        {
          tft.setTextColor(KOLOR_ZNAKOW);
          tft.fillRect(11 * charWidth, dataHeight + (alarmIndex - startAlarmIndex)*dataHeight, charWidth * 3, charHeight, KOLOR_TLA);
          --alarmIndex;
          tft.setCursor(11 * charWidth, dataHeight + (alarmIndex - startAlarmIndex)*dataHeight);
          tft.print("<--");
        }
      }
    }
    // Toggle alarm (lewy enkoder - przycisk)
    if (digitalRead(IO_encoderLkey) == LOW)
    {
      if (alarmy[alarmIndex].isActive)
        tft.fillRect(9 * charWidth, dataHeight + (alarmIndex - startAlarmIndex)*dataHeight, charWidth, charHeight, KOLOR_TLA);
      else
      {
        tft.setTextColor(KOLOR_ZNAKOW);
        tft.setCursor(9 * charWidth, dataHeight + (alarmIndex - startAlarmIndex)*dataHeight);
        tft.print("X");
      }
      alarmy[alarmIndex].isActive = !alarmy[alarmIndex].isActive;
      while (digitalRead(IO_encoderLkey) == LOW) {}
      delay(pressDelayTime);
    }

    // Obrót prawym enkoderem (Wybór opcji)
    result_EncR = EncR.process();
    if (result_EncR == DIR_CW) // w prawo
    {
      if (optionIndex < 2)
        ++optionIndex;
      if (alarmy.size() == 0 && optionIndex == 2)
        optionIndex = 0;
      drawOption_Alarms(charHeight, optionIndex);
    } else if (result_EncR == DIR_CCW)  // w lewo
    {
      if (optionIndex > 0)
        --optionIndex;
      drawOption_Alarms(charHeight, optionIndex);
    }
    // Obsługa opcji (prawy enkoder - przycisk)
    if (digitalRead(IO_encoderRkey) == LOW)
    {
      stayHere = false;
      doOption_Alarms(optionIndex, alarmIndex); // może ustawić flage globalną returnToAlarmsMenu (opcje 1 i 2)
    }
  }
}

void setOffAlarm()
{
  const char* streamUrl = "https://www.soundhelix.com/examples/mp3/SoundHelix-Song-1.mp3";
  audio.connecttohost(streamUrl);

  int textWidth = 7 * 6 * textSizeLarge;
  int textHeight = 8 * textSizeLarge;
  int x = (TFT_WIDTH - textWidth) / 2;
  int y = (TFT_HEIGHT - textHeight) / 2;

  // Wyświetl godzinę kliknięcia
  tft.fillScreen(KOLOR_TLA);
  tft.setTextSize(textSizeLarge);
  tft.setTextColor(KOLOR_ZNAKOW);
  tft.setCursor(x, y);
  tft.print("Budzik!");

  while (true) 
  {
    audio.loop(); // zagraj muzyke
    vTaskDelay(1); // to zapobiega dystorcji dźwięku
    if (!digitalRead(IO_button))
    {
      audio.stopSong();
      while (digitalRead(IO_button) == LOW) {}
      delay(pressDelayTime);
      break;
    }
  }
}

void displayVol()
{
  int charHeight = 8 * textSizeLarge;
  int charWidth = 6 * textSizeLarge;
  tft.fillRect(0, 0, charWidth * 4, charHeight, KOLOR_TLA);
  tft.setCursor(0, 0);
  tft.setTextSize(textSizeLarge);
  tft.setTextColor(KOLOR_ZNAKOW);
  String vol = "V=" + String(cur_volume);
  tft.print(vol);
}

// ****************** SETUP ****************** //
void setup() 
{
  Serial.begin(115200);

  Wire.begin(); // I2C comms
  setup_pins();
  setup_Devices();
  //setup_song();
  

  // testowe
  alarmDate t1 = {1, 45, 30, true};  // godzina: 12, minuty: 45, sekundy: 30
  alarmDate t2 = {2, 22, 59, false};   // godzina: 8, minuty: 22, sekundy: 59
  alarmDate t3 = {3, 59, 5, true};   // godzina: 23, minuty: 59, sekundy: 5
  alarmDate t4 = {4, 10, 15, false};   // godzina: 6, minuty: 10, sekundy: 15
  alarmDate t5 = {5, 30, 0, false};   // godzina: 19, minuty: 30, sekundy: 0
  alarmDate t6 = {6, 59, 50, false};
  alarmDate t7 = {7, 21, 1, false};
  alarmDate t8 = {8, 47, 8, false};
  alarmy.push_back(t1);
  alarmy.push_back(t2);
  alarmy.push_back(t3);
  alarmy.push_back(t4);
  alarmy.push_back(t5);
  alarmy.push_back(t6);
  alarmy.push_back(t7);
  alarmy.push_back(t8);

  displayVol();
}
 
// ****************** LOOP ****************** //
int lastSecond = RTC.getSecond();
void loop() 
{
  // Powroty (refreshowanie) menu - największy piorytet
  if (returnToAlarmsMenu)
  {
    returnToAlarmsMenu = false;
    display_Alarms();
    displayVol();
  }

  if (RTC.getSecond() != lastSecond)
  { // Wyświetl godzinę - wyświetlane bez inputu
    lastSecond = RTC.getSecond();
    tft_displayMenuTime();
  }
  else if (digitalRead(IO_encoderLkey) == LOW) // Wejdź do menu ustawiania alarmów
  {
    while (digitalRead(IO_encoderLkey) == LOW) {}
    delay(pressDelayTime);
    menuFunction(MainMenu::mainMenuName, MainMenu::optionsAmount, &MainMenu::optionString, MainMenu::indexesToBlock, MainMenu::items, textSizeBig, MainMenu::noItemsMsg, &MainMenu::EncL_Key, &MainMenu::EncR_Key);
    displayVol();
  }

  // Obrót lewym enkoderem (zmiana głośności)
  result_EncL = EncL.process();
  if (result_EncL == DIR_CW) // w prawo - zwiększ
  {
    if (cur_volume < 10)
      ++cur_volume;
    displayVol();
  } 
  else if (result_EncL == DIR_CCW)  // w lewo - zmniejsz
  {
    if (cur_volume > 0)
      --cur_volume;
    displayVol();
  }

  // Sprawdź czy któryś z alarmów ma się odpalić i go włącz jeśli trzeba
  bool a1, a2;
  a1 = false;
  for (int i=0; i < alarmy.size(); ++i)
  {
    if (RTC.getHour(a1, a2) == alarmy[i].h && RTC.getMinute() == alarmy[i].m && RTC.getSecond() == alarmy[i].s && alarmy[i].isActive)
    {
      setOffAlarm();
      displayVol();
    }
  }
}


