#ifndef Akta3d_TFT_Button_h
#define Akta3d_TFT_Button_h

// for better result need set 
// #define NUMSAMPLES 5 on Adafruit TouchScreen.cpp

#include "Arduino.h"
#include <Akta3d_TFT_Utils.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>


class Akta3d_TFT_Button
{
  public:
  	Akta3d_TFT_Button(/*
                      void (*cbOnPressed)() = NULL,
                      void (*cbOnReleased)()  = NULL
                      */);

    void initButtonUL( 
          MCUFRIEND_kbv *tft,
          uint16_t x,
          uint16_t y, 
          uint16_t w, 
          uint16_t h, 
          uint16_t outlineColor,
          uint16_t fillColor,
          uint16_t textColor,
          String text,
          uint16_t textSize = 2,
          uint16_t radius = 3
          );

  	void update(Akta3d_Touch_Info touchInfo);
  	void drawButton();

    // buttons helpers
    static void updateButtonList(Akta3d_TFT_Button **buttons, Akta3d_Touch_Info info);

  private:

    MCUFRIEND_kbv *_tft;
    uint16_t _x;
    uint16_t _y;
    uint16_t _w;
    uint16_t _h;
    uint16_t _outlineColor;
    uint16_t _fillColor;
    uint16_t _textColor;
    uint16_t _textSize;
    uint16_t _radius;
    String _text;
    /*void (*_cbOnPressed)();
    void (*_cbOnReleased)();*/


  // method/properties for debounce : from bounce2 library
  private:
    inline void changeState();
    inline void setStateFlag(const uint8_t flag)    {state |= flag;}
    inline void unsetStateFlag(const uint8_t flag)  {state &= ~flag;}
    inline void toggleStateFlag(const uint8_t flag) {state ^= flag;}
    inline bool getStateFlag(const uint8_t flag)    {return((state & flag) != 0);}

  public: 
    void setDebounceInterval(uint16_t interval_millis);
    bool justPressed();
    bool justReleased();

  protected:
    unsigned long previous_millis;
    uint16_t interval_millis;
    uint8_t state;
    unsigned long stateChangeLastTime;
    unsigned long durationOfPreviousState;

  private:
    static const uint8_t DEBOUNCED_STATE = 0b00000001;
    static const uint8_t UNSTABLE_STATE  = 0b00000010;
    static const uint8_t CHANGED_STATE   = 0b00000100;

};

#endif
