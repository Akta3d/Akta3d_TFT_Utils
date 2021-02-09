#ifndef Akta3d_TFT_HSlider_h
#define Akta3d_TFT_HSlider_h

#include "Arduino.h"
#include <Akta3d_TFT_Utils.h>
#include <Akta3d_TFT_Colors.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>


class Akta3d_TFT_HSlider
{
  public:
  	Akta3d_TFT_HSlider();

    void initHSliderUL( 
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
          String text = "Value",
          uint16_t textColor = WHITE,
          uint16_t textBackgroundColor = BLACK,
          uint16_t textSize = 2,
          uint16_t marginText = 8 // space between text and slider
          );

  	void update(Akta3d_Touch_Info touchInfo);
  	void drawHSlider();
    int getValue() {return _value;};
    bool hasChanged() {return _changed;}; // return true if value changed during update

  private:
    void drawSlider();
    void drawValue();
    void drawCursor(uint16_t color);

  private:

    MCUFRIEND_kbv *_tft;
    uint16_t _x;
    uint16_t _y;
    uint16_t _w;
    uint16_t _h;
    uint16_t _min;
    uint16_t _max;
    uint16_t _outlineColor;
    uint16_t _fillColor;    
    uint16_t _cursorColor;
    uint16_t _textColor;
    uint16_t _textBackgroundColor;
    uint16_t _textSize;
    uint16_t _marginText;
    String _text;
    uint16_t _nbCharForValue; // nbChar needed to write value : "-30" = 3;
    bool _withText;

    uint16_t _xSlider;
    uint16_t _ySlider;
    uint16_t _hSlider;
    uint16_t _wSlider;
    uint16_t _xText;
    uint16_t _yText;
    uint16_t _hText;
    uint16_t _wText;

    int _value;
    bool _changed;

};

#endif
