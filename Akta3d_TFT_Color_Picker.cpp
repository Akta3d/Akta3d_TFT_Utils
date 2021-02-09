#include "Akta3d_TFT_Color_Picker.h"

Akta3d_TFT_Color_Picker::Akta3d_TFT_Color_Picker(MCUFRIEND_kbv *tft) {
  _tft = tft;
  _isDisplayed = false;

  _buttons = new Akta3d_TFT_Button*[3]{
      &_okButton, 
      &_koButton, 
      NULL
  };

  initHsCanvas();
}

void Akta3d_TFT_Color_Picker::display(HSV color, void (*cbOnCancel)(), void (*cbOnSelect)(HSV)) {
  _selectedColor = color;

  _cbOnCancel = cbOnCancel;
  _cbOnSelect = cbOnSelect;

  _isDisplayed = true;
  drawInterface();
  drawSelectedColor(WHITE);
}

void Akta3d_TFT_Color_Picker::update(Akta3d_Touch_Info touchInfo) {
  if(!_isDisplayed) {
    return;
  }

  // check buttons click
  Akta3d_TFT_Button::updateButtonList(_buttons, touchInfo);

  if (_okButton.justReleased()) {             
      _cbOnSelect(_selectedColor);
      hide();
  }
  if (_koButton.justReleased()) {
    _cbOnCancel();
    hide();
  }  

  if(touchInfo.down) {
    //check HS selection
    if(touchInfo.x >= HS_START_X && touchInfo.x <= HS_START_X + HSV_SIZE && 
        touchInfo.y >= HS_START_Y && touchInfo.y <= HS_START_Y + HSV_SIZE) {
      uint16_t h = map(touchInfo.x - HS_START_X, 0, HSV_SIZE, 0, 359);
      uint16_t s = map(touchInfo.y - HS_START_Y, 0, HSV_SIZE, 100, 0);

      drawSelectedColor(AK_BACKGROUND);
      _selectedColor = {h, s, _selectedColor.v};
      drawSelectedColor(WHITE);
    }

    //check HS selection
    if(touchInfo.x >= V_START_X && touchInfo.x <= V_START_X + V_WIDTH && 
        touchInfo.y >= V_START_Y && touchInfo.y <= V_START_Y + HSV_SIZE) {
      uint16_t v = map(touchInfo.y - V_START_Y, 0, HSV_SIZE, 100, 0);

      drawSelectedColor(AK_BACKGROUND);
      _selectedColor = {_selectedColor.h, _selectedColor.s, v};
      drawSelectedColor(WHITE);
    }
  }
}

void Akta3d_TFT_Color_Picker::initHsCanvas() {
#if USE_GFX_CANVAS_16
  _hsvCanvas = new GFXcanvas16(HSV_SIZE + 1, HSV_SIZE + 1);

  // draw Hue / Saturation box
  uint16_t v = 100;
  for(int16_t x = 0 ; x < HSV_SIZE ; x += 1) {
    uint16_t h = map(x, 0, HSV_SIZE, 0, 359);
    for(int16_t y = 0 ; y < HSV_SIZE ; y += 1) {
      uint16_t s = map(y, 0, HSV_SIZE, 100, 0);
      uint16_t color = Akta3d_TFT_Utils::hsvToHex({h, s, v});
      _hsvCanvas->drawPixel(x, y, color);
    }
  }

  //_hsvCanvas->byteSwap();
/*
  _hsvCanvas = new GFXcanvas16(50, 50);
  _hsvCanvas->fillScreen(RED);
*/
#endif
}

void Akta3d_TFT_Color_Picker::drawInterface() {
  _tft->fillScreen(AK_BACKGROUND);

  // draw Hue / Saturation box
#if USE_GFX_CANVAS_16  
  _tft->drawRGBBitmap(HS_START_X, HS_START_Y, _hsvCanvas->getBuffer(), HSV_SIZE, HSV_SIZE);
  //_tft->drawRGBBitmap(HS_START_X, HS_START_Y, _hsvCanvas->getBuffer(), 50, 50);
#else
  uint16_t v = 100;
  for(uint16_t x = 0 ; x < HSV_SIZE ; x += 1) {
    uint16_t h = map(x, 0, HSV_SIZE, 0, 359);
    for(uint16_t y = 0 ; y < HSV_SIZE ; y += 1) {
      uint16_t s = map(y, 0, HSV_SIZE, 100, 0);
      uint16_t color = Akta3d_TFT_Utils::hsvToHex({h, s, v});
      _tft->drawPixel(HS_START_X + x, HS_START_Y + y, color);
    }
  }
#endif  

  // draw value box
  _tft->drawRect(V_START_X, V_START_Y, V_WIDTH, HSV_SIZE, WHITE);
  for(uint16_t y = 0 ; y < HSV_SIZE - 2 ; y += 1) {
    uint16_t value = map(y, 0, HSV_SIZE - 2, 255, 0);
    RGB rgbColor = {value, value, value};
    uint16_t color = Akta3d_TFT_Utils::rgbToHex(rgbColor);
    _tft->drawFastHLine(V_START_X + 1, V_START_Y + 1 + y, V_WIDTH - 2, color);
  }

  // draw buttons
  _tft->drawFastVLine(290, 0, 240, AK_COLOR_LINE);

  _koButton.initButtonUL(_tft,  293, 10, 24, 24, AK_COLOR_BTN_OUTLINE, AK_COLOR_BTN_KO_FILL, AK_COLOR_BTN_TEXT, "X", 1);
  _koButton.drawButton();

  _okButton.initButtonUL(_tft,  293, 204, 24, 24, AK_COLOR_BTN_OUTLINE, AK_COLOR_BTN_OK_FILL, AK_COLOR_BTN_TEXT, "OK", 1);
  _okButton.drawButton();  

  // draw selected color  
  _tft->drawRect(295, 70, 20, 100, WHITE);  
}

void Akta3d_TFT_Color_Picker::drawSelectedColor(uint16_t cursorColor) {
  // draw H cursor
  uint16_t cursorHx = map(_selectedColor.h, 0, 359, 0, HSV_SIZE);
  _tft->drawFastVLine(HS_START_X + cursorHx, HS_START_Y, -5, cursorColor);

  // draw S cursor
  uint16_t cursorSy = map(_selectedColor.s, 0, 100, HSV_SIZE, 0);
  _tft->drawFastHLine(HS_START_X, HS_START_Y + cursorSy, -5, cursorColor);

  // draw V cursor
  uint16_t cursorVy = map(_selectedColor.v, 0, 100, HSV_SIZE, 0);
  _tft->drawFastHLine(V_START_X + V_WIDTH, V_START_Y + cursorVy, 5, cursorColor);

  // draw selected color  
  _tft->fillRect(295, 70, 20, 100, Akta3d_TFT_Utils::hsvToHex(_selectedColor));
}

void Akta3d_TFT_Color_Picker::hide() {
  _isDisplayed = false;
}
