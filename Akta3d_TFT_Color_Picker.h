#ifndef Akta3d_TFT_Color_Picker_h
#define Akta3d_TFT_Color_Picker_h

#include "Arduino.h"
#include <Akta3d_TFT_Colors.h>
#include <Akta3d_TFT_Button.h>
#include <Akta3d_TFT_Utils.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

#define USE_GFX_CANVAS_16 false

class Akta3d_TFT_Color_Picker
{
  private:
    const uint16_t HSV_SIZE = 220;
    const uint16_t HS_START_X = 10;
    const uint16_t HS_START_Y = 10;
    const uint16_t V_WIDTH = 40;
    const uint16_t V_START_X = 240;
    const uint16_t V_START_Y = 10;
    
  public:
  	Akta3d_TFT_Color_Picker(MCUFRIEND_kbv *tft);
  	void display(HSV color, void (*cbOnCancel)(), void (*cbOnSelect)(HSV));
  	void update(Akta3d_Touch_Info touchInfo);

  private:
    void initHsCanvas();
  	void drawInterface();
    void drawSelectedColor(uint16_t cursorColor);

  	void hide();

  	bool _isDisplayed;
  	HSV _selectedColor;

  	MCUFRIEND_kbv *_tft;

  	void (*_cbOnCancel)();
  	void (*_cbOnSelect)(HSV);
  	
  	// define buttons
	  Akta3d_TFT_Button _okButton, _koButton;	
	  Akta3d_TFT_Button **_buttons;

    GFXcanvas16 *_hsvCanvas;
};

#endif
