#include "menuFunc.h"

void menuFunction_drawMenuBars(uint8_t menubg_charHeight, uint8_t encR_optionsAmount, const char* menuName, const char** encR_optionsNames)
{
  tft.setTextSize(textSizeMedium);
  tft.fillRect(0, 0, TFT_WIDTH, menubg_charHeight, KOLOR_MENU);
  tft.fillRect(0, TFT_HEIGHT - menubg_charHeight, TFT_WIDTH, menubg_charHeight, KOLOR_MENU);
  tft.setCursor(2, 1);
  tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
  tft.print(menuName);
  tft.setCursor(0, TFT_HEIGHT - menubg_charHeight + 2);
  switch(encR_optionsAmount)
  {
    case 1:
      tft.print(encR_optionsNames[0]);
      break;
    case 2:
      tft.print(encR_optionsNames[0]);
      tft.setTextColor(KOLOR_OPCJI);
      tft.print(encR_optionsNames[1]);
      break;
    case 3:
      tft.print(encR_optionsNames[0]);
      tft.setTextColor(KOLOR_OPCJI);
      tft.print(encR_optionsNames[1]);
      tft.print(encR_optionsNames[2]);
      break;
    default:
      return;
  }
}

void menuFunction_drawInitialData(uint8_t charW, uint8_t charH, uint8_t breakH, uint8_t menubg_dataH, uint8_t maxItemsOnScreen, uint8_t lastCharInRowIndex, const std::vector<String>& items, const char* noItemsMsg)
{
  uint8_t textSize = charW/TFT_TEXT_WIDTH;
  tft.setTextSize(textSize);

  tft.setTextColor(KOLOR_ZNAKOW);
  if (items.empty())
  {
    tft.setCursor(0, menubg_dataH);
    tft.print(noItemsMsg);
  }
  else
  {
    if (textSize < 4)
    {
      tft.setCursor((lastCharInRowIndex-2) * charW, menubg_dataH);
      tft.print("<--");
    }
    else
    {
      tft.setCursor(lastCharInRowIndex * charW, menubg_dataH);
      tft.print("<");
    }

    int i=0;
    for (; i < (maxItemsOnScreen - 1) && i < items.size(); ++i)
    {
      tft.setCursor(0, menubg_dataH + i*(charH+breakH));
      tft.print(items[i]);
    }
    if (items.size() > i)
    {
      tft.setCursor(0, menubg_dataH + i*(charH+breakH));
      if (textSize < 4)
        tft.print("vvvvvvvv");
      else
        tft.print("vvvvvv");
    }
  }
}

void menuFunction_redrawMenu(uint8_t menubg_charHeight, uint8_t encR_optionsAmount, uint8_t charW, uint8_t charH, uint8_t breakH, uint8_t menubg_dataH, uint8_t maxItemsOnScreen, uint8_t lastCharInRowIndex, 
                             const std::vector<String>& items, const char* menuName, const char** encR_optionsNames, const char* noItemsMsg, uint16_t& itemIndex, uint16_t& startItemIndex, uint8_t& optionIndex)
{
  tft.fillScreen(KOLOR_TLA);
  
  // Utworzenie pasków menu
  menuFunction_drawMenuBars(menubg_charHeight, encR_optionsAmount, menuName, encR_optionsNames);

  // Wypełnienie danymi początkowymi
  menuFunction_drawInitialData(charW, charH, breakH, menubg_dataH, maxItemsOnScreen, lastCharInRowIndex, items, noItemsMsg);

  // Zresetowanie zmiennych indexów
  itemIndex = 0;
  startItemIndex = 0;
  optionIndex = 0;
}

void menuFunction_drawSelectedItems(uint8_t charW, uint8_t charH, uint8_t breakH, uint8_t menubg_dataH, uint8_t maxItemsOnScreen, uint8_t lastCharInRowIndex, uint16_t startIndex, const std::vector<String>& items)
{
  uint8_t textSize = charW/TFT_TEXT_WIDTH;
  uint8_t menubg_charH = menubg_dataH - breakH;
  tft.setTextSize(textSize);

  tft.fillRect(0, menubg_charH, charW * (lastCharInRowIndex+1) - 1, TFT_HEIGHT-2*menubg_charH-1, KOLOR_TLA); // wyczyść itemsy
  tft.setTextColor(KOLOR_ZNAKOW);
  int i=startIndex; // indeks itemu w wektorze
  int j=0;          // pozycja do rysowania
  for (; j < (maxItemsOnScreen - 1); ++i, ++j)
  {
    tft.setCursor(0, menubg_dataH + j*(charH+breakH));
    tft.print(items[i]);
  }
  if (items.size() > i)
  {
    tft.setCursor(0, menubg_dataH + i*(charH+breakH));
    if (textSize < 4)
      tft.print("vvvvvvvv");
    else
      tft.print("vvvvvv");
  }
}

void menuFunction_EncL_CW(uint8_t charW, uint8_t charH, uint8_t breakH, uint8_t menubg_dataH, uint8_t maxItemsOnScreen, uint8_t lastCharInRowIndex, uint16_t& itemIndex, uint16_t& startItemIndex, const std::vector<String>& items)
{
  uint8_t textSize = charW/TFT_TEXT_WIDTH;
  tft.setTextSize(textSize);

  if (itemIndex < items.size() - 1)
  {
    if (itemIndex - startItemIndex == maxItemsOnScreen-2) // załaduj kolejny item (przerysuj itemsy); strzałka pozostaje w tym samym miejscu
    {
      ++startItemIndex;
      ++itemIndex;

      menuFunction_drawSelectedItems(charW, charH, breakH, menubg_dataH, maxItemsOnScreen, lastCharInRowIndex, startItemIndex, items);
    }
    else // Zmień tylko pozycję strzałki
    {
      tft.setTextColor(KOLOR_ZNAKOW);
      if (textSize < 4)
      {
        tft.fillRect((lastCharInRowIndex-2) * charW, menubg_dataH + (itemIndex - startItemIndex)*(charH+breakH), charW * 3, charH, KOLOR_TLA);
        ++itemIndex;
        tft.setCursor((lastCharInRowIndex-2) * charW, menubg_dataH + (itemIndex - startItemIndex)*(charH+breakH));
        tft.print("<--");
      }
      else
      {
        tft.fillRect(lastCharInRowIndex * charW, menubg_dataH + (itemIndex - startItemIndex)*(charH+breakH), charW, charH, KOLOR_TLA);
        ++itemIndex;
        tft.setCursor(lastCharInRowIndex * charW, menubg_dataH + (itemIndex - startItemIndex)*(charH+breakH));
        tft.print("<");
      }
    }
  }
}

void menuFunction_EncL_CCW(uint8_t charW, uint8_t charH, uint8_t breakH, uint8_t menubg_dataH, uint8_t maxItemsOnScreen, uint8_t lastCharInRowIndex, uint16_t& itemIndex, uint16_t& startItemIndex, const std::vector<String>& items)
{
  uint8_t textSize = charW/TFT_TEXT_WIDTH;
  tft.setTextSize(textSize);

  if (itemIndex > 0)
  {
    if (itemIndex == startItemIndex) // załaduj poprzednie itemsy i porusz strzałką
    {
      tft.setTextColor(KOLOR_ZNAKOW);
      --startItemIndex;
      if (textSize < 4)
      {
        tft.fillRect((lastCharInRowIndex-2) * charW, menubg_dataH + (itemIndex - startItemIndex)*(charH+breakH), charW * 3, charH, KOLOR_TLA);
        --itemIndex;
        tft.setCursor((lastCharInRowIndex-2) * charW, menubg_dataH + (itemIndex - startItemIndex)*(charH+breakH));
        tft.print("<--");
      }
      else
      {
        tft.fillRect(lastCharInRowIndex * charW, menubg_dataH + (itemIndex - startItemIndex)*(charH+breakH), charW, charH, KOLOR_TLA);
        --itemIndex;
        tft.setCursor(lastCharInRowIndex * charW, menubg_dataH + (itemIndex - startItemIndex)*(charH+breakH));
        tft.print("<");
      }

      menuFunction_drawSelectedItems(charW, charH, breakH, menubg_dataH, maxItemsOnScreen, lastCharInRowIndex, startItemIndex, items);
    }
    else // Zmień tylko pozycję strzałki
    {
      tft.setTextColor(KOLOR_ZNAKOW);
      if (textSize < 4)
      {
        tft.fillRect((lastCharInRowIndex-2) * charW, menubg_dataH + (itemIndex - startItemIndex)*(charH+breakH), charW * 3, charH, KOLOR_TLA);
        --itemIndex;
        tft.setCursor((lastCharInRowIndex-2) * charW, menubg_dataH + (itemIndex - startItemIndex)*(charH+breakH));
        tft.print("<--");
      }
      else
      {
        tft.fillRect(lastCharInRowIndex * charW, menubg_dataH + (itemIndex - startItemIndex)*(charH+breakH), charW, charH, KOLOR_TLA);
        --itemIndex;
        tft.setCursor(lastCharInRowIndex * charW, menubg_dataH + (itemIndex - startItemIndex)*(charH+breakH));
        tft.print("<");
      }
    }
  }
}

// Tutaj optionsAmount to albo 2 albo 3
void menuFunction_drawOptions(uint8_t menubg_charH, uint8_t selectedOption, uint8_t optionsAmount, const char** par_encR_optionsNames)
{
  // Wyczyść opcje
  tft.fillRect(0, TFT_HEIGHT - menubg_charH, TFT_WIDTH, menubg_charH, KOLOR_MENU);
  tft.setCursor(0, TFT_HEIGHT - menubg_charH + 2);
  tft.setTextColor(KOLOR_OPCJI);
  tft.setTextSize(textSizeMedium);

  // Wyrysuj na nowo z odpowiednim podkreśleniem
  if (optionsAmount == 2)
  {
    switch (selectedOption)
    {
      case 0:
        tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
        tft.print(par_encR_optionsNames[0]);
        tft.setTextColor(KOLOR_OPCJI);
        tft.print(par_encR_optionsNames[1]);
        return;
      case 1:
        tft.print(par_encR_optionsNames[0]);
        tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
        tft.print(par_encR_optionsNames[1]);
        return;
    }
  }
  else // 3
  {
    switch (selectedOption)
    {
      case 0:
        tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
        tft.print(par_encR_optionsNames[0]);
        tft.setTextColor(KOLOR_OPCJI);
        tft.print(par_encR_optionsNames[1]);
        tft.print(par_encR_optionsNames[2]);
        return;
      case 1:
        tft.print(par_encR_optionsNames[0]);
        tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
        tft.print(par_encR_optionsNames[1]);
        tft.setTextColor(KOLOR_OPCJI);
        tft.print(par_encR_optionsNames[2]);
        return;
      case 2:
        tft.print(par_encR_optionsNames[0]);
        tft.print(par_encR_optionsNames[1]);
        tft.setTextColor(KOLOR_WYBRANEJ_OPCJI);
        tft.print(par_encR_optionsNames[2]);
        return;
    }
  }
}

void menuFunction_EncR_CW(uint8_t menubg_charH, const char** optionsNames, uint8_t optionsAmount, uint8_t& optionIndex, const std::vector<String>& items, const std::vector<uint8_t>& indexesToBlock, uint8_t indexToRedirectTo=0)
{
  if (optionsAmount == 1) return;
  if (optionIndex == optionsAmount - 1) return;

  if (optionIndex < optionsAmount)
    ++optionIndex;

  int i=0;
  if (items.size() == 0)
  {
    for (;i < indexesToBlock.size(); ++i)
    {
      if (optionIndex == indexesToBlock[i])
      {
        optionIndex = indexToRedirectTo;
        break;
      }
    }
  }
  menuFunction_drawOptions(menubg_charH, optionIndex, optionsAmount, optionsNames);
}

void menuFunction_EncR_CCW(uint8_t menubg_charH, const char** optionsNames, uint8_t optionsAmount, uint8_t& optionIndex)
{
  if (optionsAmount == 1) return;
  if (optionIndex == 0) return;

  if (optionIndex > 0)
    --optionIndex;
  menuFunction_drawOptions(menubg_charH, optionIndex, optionsAmount, optionsNames);
}

// Max. opcji to 3, no i wiadomo tekst musi się zmieścić; w stringach opcji trzeba pamiętać o spacjach
void menuFunction(const char* par_menuName, uint8_t par_encR_optionsAmount, const char** par_encR_optionsNames, std::vector<uint8_t>& indexesToBlock, std::vector<String>& par_items, 
                  uint8_t par_itemTextSize, const char* par_noItemsMsg, bool (*par_EncL_Key_fcnptr)(uint16_t&, uint16_t&), bool (*par_EncR_Key_fcnptr)(uint8_t&, uint8_t&, bool&), uint8_t par_indexToRedirectTo)
{
  tft.fillScreen(KOLOR_TLA); // wyczyść ekran na start
  uint8_t menubg_charWidth = TFT_TEXT_WIDTH * textSizeMedium;
  uint8_t menubg_charHeight = TFT_TEXT_HEIGHT * textSizeMedium;
  uint8_t menubg_breakHeight = TFT_TEXT_HEIGHT;
  uint8_t menubg_dataHeight = menubg_charHeight + menubg_breakHeight;
  uint8_t item_charWidth = TFT_TEXT_WIDTH * par_itemTextSize;
  uint8_t item_charHeight = TFT_TEXT_HEIGHT * par_itemTextSize;
  uint8_t maxItemsOnScreen = (TFT_HEIGHT - 2 * menubg_charHeight)/(TFT_TEXT_HEIGHT*(1 + par_itemTextSize));
  uint8_t lastCharInRowIndex = TFT_WIDTH/item_charWidth - 1;
  uint16_t itemIndex = 0;
  uint16_t startItemIndex = 0;
  uint8_t optionIndex = 0;
  bool stayHere = true;

  // Utworzenie pasków menu
  menuFunction_drawMenuBars(menubg_charHeight, par_encR_optionsAmount, par_menuName, par_encR_optionsNames);
  // Wypełnienie danymi początkowymi
  menuFunction_drawInitialData(item_charWidth, item_charHeight, menubg_breakHeight, menubg_dataHeight, maxItemsOnScreen, lastCharInRowIndex, par_items, par_noItemsMsg);

  // Dynamiczne wyświetlanie danych i obsługa inputu użytkownika
  while(stayHere)
  {
    bool doRedraw=false;
    // Obrót lewym enkoderem (Wybór itemu)
    result_EncL = EncL.process();
    if (result_EncL == DIR_CW)                        // w prawo
      menuFunction_EncL_CW(item_charWidth, item_charHeight, menubg_breakHeight, menubg_dataHeight, maxItemsOnScreen, lastCharInRowIndex, itemIndex, startItemIndex, par_items);
    else if (result_EncL == DIR_CCW)                  // w lewo
      menuFunction_EncL_CCW(item_charWidth, item_charHeight, menubg_breakHeight, menubg_dataHeight, maxItemsOnScreen, lastCharInRowIndex, itemIndex, startItemIndex, par_items);

    // Obsługa itemu (lewy enkoder - przycisk)
    if (digitalRead(IO_encoderLkey) == LOW && par_EncL_Key_fcnptr != nullptr)
    {
      doRedraw = par_EncL_Key_fcnptr(itemIndex, startItemIndex);
      // Po wyjściu z kolejnej funkcji trzeba od nowa narysować tą.
      if (doRedraw) menuFunction_redrawMenu(menubg_charHeight, par_encR_optionsAmount, item_charWidth, item_charHeight, menubg_breakHeight, menubg_dataHeight, maxItemsOnScreen, lastCharInRowIndex, par_items, par_menuName, par_encR_optionsNames, par_noItemsMsg, itemIndex, startItemIndex, optionIndex);
    }

    // Obrót prawym enkoderem (Wybór opcji)
    result_EncR = EncR.process();
    if (result_EncR == DIR_CW)                        // w prawo
      menuFunction_EncR_CW(menubg_charHeight, par_encR_optionsNames, par_encR_optionsAmount, optionIndex, par_items, indexesToBlock, par_indexToRedirectTo);
    else if (result_EncR == DIR_CCW)                  // w lewo
      menuFunction_EncR_CCW(menubg_charHeight, par_encR_optionsNames, par_encR_optionsAmount, optionIndex);

    // Obsługa opcji (prawy enkoder - przycisk)
    if (digitalRead(IO_encoderRkey) == LOW && par_EncR_Key_fcnptr != nullptr)
    {
      doRedraw = par_EncR_Key_fcnptr(optionIndex, par_encR_optionsAmount, stayHere);
      if (doRedraw) menuFunction_redrawMenu(menubg_charHeight, par_encR_optionsAmount, item_charWidth, item_charHeight, menubg_breakHeight, menubg_dataHeight, maxItemsOnScreen, lastCharInRowIndex, par_items, par_menuName, par_encR_optionsNames, par_noItemsMsg, itemIndex, startItemIndex, optionIndex);
    }
  }
}

uint8_t sufit(float x)
{
  uint8_t statcast = static_cast<uint8_t>(x);
  if (x > static_cast<float>(statcast))
    return statcast + 1;
  else
    return statcast;
}

struct FormattedText_t {
  std::vector<String> lines;
  std::vector<bool> leftConnector;
  std::vector<bool> rightConnector;
};

struct FormattedText_t* getFormattedText(const String& text, uint8_t charsInLine)
{
  struct FormattedText_t* fText = new struct FormattedText_t;
  if (!fText)
    return nullptr;

  for (uint8_t passes = 0;; ++passes)
  {
    bool doBreak = false;
    bool sthWasRead = false;
    String tempLine;
    tempLine.reserve(charsInLine);
    for (uint16_t i = passes * charsInLine; i < charsInLine * (passes + 1); ++i)
    {
      if (i < text.length())
      {
        tempLine += text[i];
        sthWasRead = true;
      }     
      else // Koniec czytania tekstu
      {
        doBreak = true;
        break;
      }
    }
    if (sthWasRead)
    {
      fText->lines.push_back(tempLine);
      fText->rightConnector.push_back(false);
      fText->leftConnector.push_back(false);
    }
    if (doBreak)
      break;
  }

  switch (fText->lines[0][fText->lines[0].length() - 1])
  {
  case ' ': // Jak spacja, to wycentruj
    fText->lines[0].remove(fText->lines[0].length() - 1);
    break;
  case '.': case ';': case ':': // jak taki znak to nic nie rób
    break;
  default: // Jak litera/cyfra to jeśli następna to nie whitespace to daj myślnik
    if (fText->lines.size() > 1)
    {
      switch (fText->lines[1][0])
      {
      case ' ': case '.': case ';': case ':':
        break;
      default:
        fText->rightConnector[0] = true;
        fText->leftConnector[1] = true;
        break;
      }
    }
    break;
  }
  for (uint8_t i = 1; i < fText->lines.size(); ++i)
  {
    switch (fText->lines[i][fText->lines[i].length() - 1])
    {
    case ' ': // Jak spacja, to wycentruj
      fText->lines[i].remove(fText->lines[i].length() - 1);
      break;
    case '.': case ';': case ':': // jak taki znak to nic nie rób
      break;
    default: // Jak litera/cyfra to jeśli następna to nie whitespace to daj myślnik
      if (fText->lines.size() > i + 1)
      {
        switch (fText->lines[i + 1][0])
        {
        case ' ': case '.': case ';': case ':':
          break;
        default:
          fText->rightConnector[i] = true;
          fText->leftConnector[i + 1] = true;
          break;
        }
      }
      break;
    }
  }

  return fText;
}

// Dorobić przerwy między linijkami
// Dorobić żeby wypisywało w środkowych wierszach
void userConfirm(String displayText)
{
  if (displayText.length() > 900)
  {
    userConfirm("Blad: Zbyt dlugi tekst");
    return;
  } 
  if (displayText.length() == 0)
  {
    userConfirm("Blad: brak tekstu");
    return;
  }

  // Zmienne napisu
  const uint8_t textBoxHeight = 150;
  const uint16_t textBoxWidth = 300;
  const uint16_t textChars = displayText.length() * TFT_TEXT_WIDTH;
  uint8_t linesAmount;
  uint8_t charsInLine;
  uint8_t linesNeeded;
  uint8_t textSize;
  uint8_t connectorSize = 9;

  // Wybierz rozmiar czcionki
  uint8_t textSizes[5] = { textSizeLarge, textSizeBig, textSizeMedium, textSizeSmallish, textSizeSmall };
  for (uint8_t i=0; i < 5; i++)
  {
    linesAmount = textBoxHeight / (textSizes[i] * TFT_TEXT_HEIGHT);
    charsInLine = textBoxWidth / (textSizes[i] * TFT_TEXT_WIDTH);
    if (displayText.length() <= linesAmount * charsInLine)
    {
      textSize = textSizes[i];
      break;
    }
  }
  if (textSize < textSizeBig)
    connectorSize = 5;

  const struct FormattedText_t* ft = getFormattedText(displayText, charsInLine);
  if (ft == nullptr)
  {
    userConfirm("Blad: nie mozna zaalokowac pamieci");
    return;
  }
  
  // Wypisz tekst - w for() pełne linie, poza for() końcówka
  tft.fillScreen(KOLOR_TLA);
  tft.setTextSize(textSize);
  tft.setTextColor(KOLOR_ZNAKOW);
  for (uint8_t i=0; i < ft->lines.size(); ++i)
  {
    const uint16_t xpos = 160 - (ft->lines[i].length() * TFT_TEXT_WIDTH * textSize) / 2;
    tft.setCursor(xpos, 10 + TFT_TEXT_HEIGHT * textSize * i);
    tft.print(ft->lines[i]);
    if (ft->leftConnector[i])
      tft.fillRect(xpos - connectorSize - 1, 10 + TFT_TEXT_HEIGHT * textSize * i + 3 * textSize, connectorSize, 2 * textSize, KOLOR_ZNAKOW);
    if (ft->rightConnector[i])
      tft.fillRect(xpos + 1 + (ft->lines[i].length() * TFT_TEXT_WIDTH * textSize), 10 + TFT_TEXT_HEIGHT * textSize * i + 3 * textSize, connectorSize, 2 * textSize, KOLOR_ZNAKOW);
  }
  
  // Zmienne okejki
  const uint8_t OK_x = (TFT_WIDTH - 2 * textSizeLarge * TFT_TEXT_WIDTH) / 2;
  const uint8_t OK_y = TFT_HEIGHT - TFT_TEXT_HEIGHT * (textSizeMedium + textSizeLarge);
  const uint8_t OK_fill_x = OK_x - TFT_TEXT_WIDTH;
  const uint8_t OK_fill_width = 2 * textSizeLarge * TFT_TEXT_WIDTH + 9;
  const uint8_t OK_fill_y = OK_y - TFT_TEXT_WIDTH;
  const uint8_t OK_fill_height = textSizeLarge * TFT_TEXT_HEIGHT + TFT_TEXT_WIDTH;
  // Zmienne Funkcjonalne
  bool stayHere = true;

  // Narysuj ok
  tft.setTextSize(textSizeLarge);
  tft.fillRect(OK_fill_x, OK_fill_y, OK_fill_width, OK_fill_height, KOLOR_MENU);
  tft.setCursor(OK_x, OK_y);
  tft.print("OK");

  // Przyciśnij którykowliek z enkoderów żeby wyjść
  while (stayHere)
  {
    if (digitalRead(IO_encoderLkey) == LOW || digitalRead(IO_encoderRkey) == LOW)
    {
      stayHere = false;
      while (digitalRead(IO_encoderLkey) == LOW || digitalRead(IO_encoderRkey) == LOW) {}
      delay(pressDelayTime);
      delete ft;
      return;
    }
  }
}