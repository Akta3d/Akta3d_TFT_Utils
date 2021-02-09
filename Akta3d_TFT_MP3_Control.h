#ifndef Akta3d_TFT_MP3_Control_h
#define Akta3d_TFT_MP3_Control_h

#include "Arduino.h"
#include <Akta3d_TFT_Colors.h>
#include <Akta3d_TFT_Utils.h>
#include <Akta3d_TFT_Button.h>
#include <Akta3d_TFT_HSlider.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

class Akta3d_TFT_MP3_Control
{
  public:
  	Akta3d_TFT_MP3_Control(MCUFRIEND_kbv *tft, uint16_t startY);
  	void update(Akta3d_Touch_Info touchInfo);
  	void drawInterface();
    uint16_t getInterfaceHeight();

  private:
    const uint16_t BTN_TEXT_SIZE = 1;
    const uint16_t BTN_H = 26;
    const uint16_t BTN_SMALL_W = 32;
    const uint16_t BTN_BIG_W = 56;
    const uint16_t BTN_MARGIN = 16;

  	MCUFRIEND_kbv *_tft;
    uint16_t _startY;
    bool _playing;
    uint16_t _loopMode; // 0:no loop, 1: loop one, 2: loop all
  	
  	// define buttons
	  Akta3d_TFT_Button _prevButton,
                        _playButton,
                        _nextButton,
                        _loopButton;
	  Akta3d_TFT_Button **_buttons;

    uint16_t _volume;
    Akta3d_TFT_HSlider _volumeSlider;
};

#endif
