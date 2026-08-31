#ifndef DEFINES_H
#define DEFINES_H

#include <Arduino.h>

// ****************** PINOUT ****************** //

// Clock (DS3231) I2C: 
// 8 - SDA
// 9 - SCL
// Encoder1 (Lewy)
constexpr uint8_t IO_encoderLkey = 4;
constexpr uint8_t IO_encoderLs1 = 1;
constexpr uint8_t IO_encoderLs2 = 2;
// Encoder2 (Prawy)
constexpr uint8_t IO_encoderRkey = 15;
constexpr uint8_t IO_encoderRs1 = 16;
constexpr uint8_t IO_encoderRs2 = 17;
// Button
constexpr uint8_t IO_button = 38;
// LCD - SPI
constexpr uint8_t IO_lcd_CS = 10; // CS, LOW - wybierz ten moduł
constexpr uint8_t IO_lcd_CLK = 12; // Zegar
constexpr uint8_t IO_lcd_DIN = 11; // (MOSI/Data Input)
// SPI ^^^^
constexpr uint8_t IO_lcd_DC = 14; // Data Command (HIGH - przesyłana jest grafika; LOW - przesyłane są polecenia)
constexpr uint8_t IO_lcd_RST = 18; // Reset (LOW resetuje moduł)
constexpr uint8_t IO_lcd_BL = 21; // Backlight (regulacja jasności)
// MicroSD - SPI też
constexpr uint8_t IO_microSD_CS = 47; // CS, LOW - wybierz ten moduł
constexpr uint8_t IO_microSD_CLK = 12; // Zegar
constexpr uint8_t IO_microSD_DI = 11; // (MOSI)
constexpr uint8_t IO_microSD_DO = 13; // (MISO)
constexpr uint8_t IO_microSD_CD = 48; // (CD - Card Detect; kiedy jest karta to pin - floating (pull up trzeba), kiedy nie ma to pin - LOW) 
// PCM5102 (I2S)
constexpr uint8_t IO_I2S_DIN = 5;
constexpr uint8_t IO_I2S_LCK = 6;
constexpr uint8_t IO_I2S_BCK = 7;

// ****************** ZMIENNE GLOBALNE ****************** //
#define KOLOR_TLA ILI9341_DARKCYAN
#define KOLOR_MENU ILI9341_CYAN
#define KOLOR_ZNAKOW ILI9341_WHITE
#define KOLOR_PODKRESLENIA 0xFEC8   // 0xFFD700 złoty
#define KOLOR_OPCJI 0xBDD7          // 0xBDD7   darkslategrey
#define KOLOR_WYBRANEJ_OPCJI 0x942D // 0xD7942D olivedrab


#define TFT_HEIGHT 240
#define TFT_WIDTH 320
#define TFT_TEXT_WIDTH 6
#define TFT_TEXT_HEIGHT 8
#define VOLUME_STEPS 20

constexpr uint8_t textSizeSmall = 1;
constexpr uint8_t textSizeSmallish = 2;
constexpr uint8_t textSizeMedium = 3;
constexpr uint8_t textSizeBig = 4;
constexpr uint8_t textSizeLarge = 5;
constexpr uint8_t pressDelayTime = 20;

struct alarmDate
{
  uint8_t h;
  uint8_t m;
  uint8_t s;
  bool isActive;
};

struct dateText_t
{
  uint8_t day;
  uint8_t month;
  uint8_t year;
  uint8_t DoW;
  String dayName;
};

struct hour_t
{
  uint8_t h;
  uint8_t m;
  uint8_t s;
};

#endif