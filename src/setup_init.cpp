#include "setup_init.h"

void setup_pins()
{
  // Encoder 1
  pinMode(IO_encoderLkey, INPUT_PULLUP);
  pinMode(IO_encoderLs1, INPUT);
  pinMode(IO_encoderLs2, INPUT);
  // Button
  pinMode(IO_button, INPUT_PULLUP);
  // Encoder 2
  pinMode(IO_encoderRkey, INPUT_PULLUP);
  pinMode(IO_encoderRs1, INPUT);
  pinMode(IO_encoderRs2, INPUT);
}

void setup_clockTime()
{
  enable_WiFi();
  RTC.setClockMode(false);

  const char* ntpServer = "tempus1.gum.gov.pl"; // Zwraca czas urzędowy w Polsce?? posixowe funkcje i tak sie rządzą swoimi zasadami
  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", ntpServer);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  RTC.setSecond(timeinfo.tm_sec);
  RTC.setMinute(timeinfo.tm_min);
  RTC.setHour(timeinfo.tm_hour);
  // Konwersja z 0-6 range do 1-7 range; tm zwraca ilosc dni od niedzieli, wiec 0=niedziela -> 1=niedziela
  RTC.setDoW(timeinfo.tm_wday + 1);
  RTC.setDate(timeinfo.tm_mday);
  // tm zwraca miesiące od stycznia; 0=styczeń; +1 bo przecież styczeń jest oznaczany jako 01
  RTC.setMonth(timeinfo.tm_mon + 1);
  // Przyjmuje wartości od 00-99; tm_year ma lata od 1900;
  RTC.setYear(timeinfo.tm_year - 100);

  disable_WiFi();
}

void setup_tft()
{
  tft.begin();
  tft.setRotation(3);
  tft.setTextWrap(true);
  tft.fillScreen(KOLOR_TLA);
}

void setup_audio()
{
  audio.setPinout(IO_I2S_BCK, IO_I2S_LCK, IO_I2S_DIN);
  audio.setVolume(cur_volume); // 0...VOLUME_STEPS
}

void setup_song()
{
  enable_WiFi();
  const char* streamUrl = "https://www.soundhelix.com/examples/mp3/SoundHelix-Song-1.mp3";
  audio.connecttohost(streamUrl);
  disable_WiFi();
}

void setup_Devices()
{
  if (false)
    setup_clockTime();
  setup_tft();
  setup_audio();
}