#include "Akta3d_TFT_MP3_Control.h"

Akta3d_TFT_MP3_Control::Akta3d_TFT_MP3_Control(MCUFRIEND_kbv *tft, uint16_t startY) {
  _tft = tft;
  _startY = startY;

  _playing = false;
  _loopMode = 0; 

  _buttons = new Akta3d_TFT_Button*[5]{
      &_prevButton, 
      &_playButton, 
      &_nextButton, 
      &_loopButton, 
      NULL
  };

  _volume = 20;
}

uint16_t Akta3d_TFT_MP3_Control::getInterfaceHeight() {
  return (BTN_H + BTN_MARGIN * 2) * 2;
}

void Akta3d_TFT_MP3_Control::update(Akta3d_Touch_Info touchInfo) {
  // update all fields/buttons
  Akta3d_TFT_Button::updateButtonList(_buttons, touchInfo);
  _volumeSlider.update(touchInfo);

  // actions
  if (_prevButton.justPressed()) {             
    Serial.println("Prev");
  }
  if (_playButton.justPressed()) {
    _playing = !_playing;
    Serial.println(_playing ? "Play" : "Pause");
    drawInterface();
  }  
  if (_nextButton.justPressed()) {             
    Serial.println("Next");
  }
  if (_loopButton.justPressed()) {             
    Serial.println("Loop");
    _loopMode += 1;
    if(_loopMode > 2) _loopMode = 0;
    drawInterface();
  }

  if(_volumeSlider.hasChanged()) {
    Serial.print("Volume : ");
    Serial.println(_volumeSlider.getValue());
  }
}

void Akta3d_TFT_MP3_Control::drawInterface() {
  uint16_t tftWidth = _tft->width();
  _tft->drawFastHLine(0, _startY, tftWidth, AK_COLOR_LINE);

  char* playBtnText = _playing ? "Pause" : "Play";
  char* loopBtnText;
  switch(_loopMode) {
    case 0:
      loopBtnText = "No loop";
    break;
    case 1:
      loopBtnText = "Loop One";
    break;
    case 2:
      loopBtnText = "Loop all";
    break;    
  };

  _prevButton.initButtonUL(_tft, 1* BTN_MARGIN + 0*BTN_SMALL_W + 0*BTN_BIG_W, _startY + BTN_MARGIN, BTN_SMALL_W, BTN_H, AK_COLOR_BTN_OUTLINE, AK_COLOR_BTN_FILL, AK_COLOR_BTN_TEXT, "<", BTN_TEXT_SIZE);
  _playButton.initButtonUL(_tft, 2* BTN_MARGIN + 1*BTN_SMALL_W + 0*BTN_BIG_W, _startY + BTN_MARGIN, BTN_BIG_W, BTN_H, AK_COLOR_BTN_OUTLINE, AK_COLOR_BTN_FILL, AK_COLOR_BTN_TEXT, playBtnText, BTN_TEXT_SIZE);
  _nextButton.initButtonUL(_tft, 3* BTN_MARGIN + 1*BTN_SMALL_W + 1*BTN_BIG_W, _startY + BTN_MARGIN, BTN_SMALL_W, BTN_H, AK_COLOR_BTN_OUTLINE, AK_COLOR_BTN_FILL, AK_COLOR_BTN_TEXT, ">", BTN_TEXT_SIZE);
  
  _loopButton.initButtonUL(_tft, tftWidth - BTN_BIG_W - BTN_MARGIN, _startY + BTN_MARGIN, BTN_BIG_W, BTN_H, AK_COLOR_BTN_OUTLINE, AK_COLOR_BTN_FILL, AK_COLOR_BTN_TEXT, loopBtnText, BTN_TEXT_SIZE);
  _prevButton.drawButton();
  _playButton.drawButton();
  _nextButton.drawButton();
  _loopButton.drawButton();

  _tft->drawFastHLine(0, _startY + BTN_H + BTN_MARGIN * 2, tftWidth, AK_COLOR_LINE);

  // draw volume slider
  _volumeSlider.initHSliderUL(_tft, BTN_MARGIN, _startY + BTN_MARGIN + getInterfaceHeight() / 2, _tft->width() - BTN_MARGIN * 2, BTN_H, 
                              0, 30, _volume,
                              AK_COLOR_BTN_OUTLINE, BLACK, AK_BLUE, "Vol");
  _volumeSlider.drawHSlider();

  _tft->drawFastHLine(0, _startY + (BTN_H + BTN_MARGIN * 2) * 2, tftWidth, AK_COLOR_LINE);
}
