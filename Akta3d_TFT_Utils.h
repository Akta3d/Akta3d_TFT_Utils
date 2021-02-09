#ifndef Akta3d_TFT_Utils_h
#define Akta3d_TFT_Utils_h

#include "Arduino.h"

#include <TouchScreen.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

#define PORTRAIT  0
#define LANDSCAPE 1

struct Akta3d_Screen_Settings {
	int xp;
	int xm;
	int yp;
	int ym;

	int tsLeft;
	int tsRight;
	int tsTop;
	int tsBottom;

	int orientation;

	int minPressure;
	int maxPressure;
};

struct Akta3d_Touch_Info {
  bool down;
  uint16_t x;
  uint16_t y;
};

struct RGB {
  uint16_t r;
  uint16_t g;
  uint16_t b;
};

struct HSV {
  uint16_t h;
  uint16_t s;
  uint16_t v;
};

class Akta3d_TFT_Utils
{
  public:
  	// colors helpers
  	static uint16_t rgbToHex(RGB rgb);

	static RGB hsvToRgb(HSV hsv);
	static HSV rgbToHsv(RGB rgb);

	static uint16_t hsvToHex(HSV hsv);
	static HSV hexToHsv(uint16_t color);

	// touchscreen helpers
  	static Akta3d_Touch_Info getTouchInfo(MCUFRIEND_kbv *tft, TouchScreen *ts, Akta3d_Screen_Settings settings);

};

#endif
