#include "Akta3d_TFT_HSlider.h"

Akta3d_TFT_HSlider::Akta3d_TFT_HSlider() {
}

void Akta3d_TFT_HSlider::initHSliderUL(
                            MCUFRIEND_kbv *tft,
                            uint16_t x,
                            uint16_t y, 
                            uint16_t w, 
                            uint16_t h,
                            int min,
                            int max, 
                            int initialValue,
                            uint16_t outlineColor,
                            uint16_t fillColor,
                            uint16_t cursorColor,
                            String text = "Value", // if empty string, don't display label and value
                            uint16_t textColor = WHITE,
                            uint16_t textBackgroundColor = BLACK,
                            uint16_t textSize = 2,
                            uint16_t marginText = 8
                      ) {
  _tft = tft;
  _x = x;
  _y = y;
  _w = w;
  _h = h;
  _min = min;
  _max = max;
  _outlineColor = outlineColor;
  _fillColor = fillColor;
  _cursorColor = cursorColor;
  _textColor = textColor;
  _textBackgroundColor = textBackgroundColor;
  _text = text;
  _textSize = textSize;
  _marginText = marginText;

  _withText = _text != "";
  _value = initialValue;
  _changed = false;

  // compute position value for text and slider
  _xText = 0;
  _yText = 0;
  _hText = 0;
  _wText = 0;
  if(_withText) {
    _tft->setFont(NULL);
    _tft->setTextSize(_textSize);

    String minText = _text + " : " + _min;
    int16_t XMin, YMin; // variables needed to use getTextBounds()
    uint16_t WMin, HMin;
    _tft->getTextBounds(minText, 0, 0, &XMin, &YMin, &WMin, &HMin);

    String maxText = _text + " : " + _max;
    int16_t XMax, YMax; // variables needed to use getTextBounds()
    uint16_t WMax, HMax;
    _tft->getTextBounds(maxText, 0, 0, &XMax, &YMax, &WMax, &HMax);

    _wText = WMax;
    _hText = HMax;
    if(WMin > WMax) {
      _wText = WMin;
      _hText = HMin;
    }

    _xText = _x;
    _yText = _y + ((_h - _hText) / 2);  

    // compute _nbCharForValue
     String maxStr = String(_max);
     String minStr = String(_min);
     _nbCharForValue = max(maxStr.length(), minStr.length());
  }

  _xSlider = _x;
  _ySlider = _y;
  _hSlider = _h;
  _wSlider = _w;
  if(_withText) {
    _xSlider = _x + _wText + _marginText;
    _wSlider = _w - (_wText + _marginText);
  }  
}    

void Akta3d_TFT_HSlider::update(Akta3d_Touch_Info touchInfo) {
  _changed = false;
  if(
      touchInfo.down && 
      touchInfo.x >= _xSlider &&
      touchInfo.x <= _xSlider + _wSlider &&
      touchInfo.y >= _ySlider &&
      touchInfo.y <= _ySlider + _hSlider
  ) {
    int newValue = map(touchInfo.x, _xSlider, _xSlider + _wSlider, _min, _max);
    if(newValue != _value) {      
      _changed = true;

      drawCursor(_fillColor);

      int lastValue = _value;
      _value = newValue;

      if((lastValue == _min && _value == _min + 1) || (lastValue == _max && _value == _max - 1)) {
        drawHSlider(); // redraw all, to redraw slider if cursor was on min/max limit
      } else {
        drawValue();
        drawCursor(_cursorColor);
      }
    }
  }
}

void Akta3d_TFT_HSlider::drawHSlider() {
  drawSlider();
  drawValue();
  drawCursor(_cursorColor);
}

void Akta3d_TFT_HSlider::drawSlider() {
  // Draw slider background
  _tft->fillRect(_xSlider , _ySlider , _wSlider , _hSlider, _fillColor); 

  // To increase speed, draw border only if it has a different color than button background
  if(_fillColor != _outlineColor) {
    _tft->drawRect(_xSlider , _ySlider , _wSlider , _hSlider, _outlineColor);
  }
}

void Akta3d_TFT_HSlider::drawValue() {
  // draw text and value
  if(_withText) {
    //write text and value
    _tft->setTextColor(_textColor, _textBackgroundColor);
    _tft->setFont(NULL);
    _tft->setTextSize(_textSize);
    _tft->setCursor(_xText , _yText);
    String valueStr = String(_value);
    for(int i = valueStr.length() ; i < _nbCharForValue ; i+=1) {
      valueStr = " " + valueStr;
    }
    String realText = _text + " : " + valueStr;
    _tft->print(realText);
  }
}

void Akta3d_TFT_HSlider::drawCursor(uint16_t color) {
  // draw cursor
  uint16_t cursorSize = _wSlider / (_max - _min);
  uint16_t xCursor = map(_value, _min, _max, _xSlider, _xSlider + _wSlider);
  _tft->fillRect(xCursor, _ySlider + 1, cursorSize, _hSlider - 2, color);
}
