#include "Akta3d_TFT_Button.h"

Akta3d_TFT_Button::Akta3d_TFT_Button(
                      /*void (*cbOnPressed)() = NULL,
                      void (*cbOnReleased)() = NULL*/
                      ) {
  /*_cbOnPressed = cbOnPressed;
  _cbOnReleased = cbOnReleased;*/

  // debounce variables
  previous_millis = 0;
  interval_millis = 35;
  state = 0;

  // bouce2::begin directly set on constructor
  /*
  state = 0;
  if (readCurrentState()) {
    setStateFlag(DEBOUNCED_STATE | UNSTABLE_STATE);
  }

  previous_millis = millis();
  */
}

void Akta3d_TFT_Button::initButtonUL(
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
                      ) {
  _tft = tft;
  _x = x;
  _y = y;
  _w = w;
  _h = h;
  _outlineColor = outlineColor;
  _fillColor = fillColor;
  _textColor = textColor;
  _text = text;
  _textSize = textSize;
  _radius = radius;
}
    
void Akta3d_TFT_Button::setDebounceInterval(uint16_t interval_millis) {
    interval_millis = interval_millis;
}

void Akta3d_TFT_Button::update(Akta3d_Touch_Info touchInfo) {
  unsetStateFlag(CHANGED_STATE);

  // Read the state of button pressed in a temporary variable.
  bool currentState = touchInfo.down && 
                      touchInfo.x > _x &&
                      touchInfo.x < _x + _w &&
                      touchInfo.y > _y &&
                      touchInfo.y < _y + _h;
  
  //Serial.print("currentState : ");
  //Serial.println(currentState);

  // If the reading is different from last reading, reset the debounce counter
  if ( currentState != getStateFlag(UNSTABLE_STATE) ) {
    previous_millis = millis();
    toggleStateFlag(UNSTABLE_STATE);
    //Serial.println("UNSTABLE_STATE");
  } else {
    if ( millis() - previous_millis >= interval_millis ) {
      // We have passed the threshold time, so the input is now stable
      // If it is different from last state, set the STATE_CHANGED flag
      if (currentState != getStateFlag(DEBOUNCED_STATE) ) {
        previous_millis = millis();               
        changeState();

        //Serial.println("CHANGE");
      }
    }
  }  
}

inline void Akta3d_TFT_Button::changeState() {
  toggleStateFlag(DEBOUNCED_STATE);
  setStateFlag(CHANGED_STATE) ;
  durationOfPreviousState = millis() - stateChangeLastTime;
  stateChangeLastTime = millis();
}
/*
bool Akta3d_TFT_Button::read() {
    return  getStateFlag(DEBOUNCED_STATE);
}*/

bool Akta3d_TFT_Button::justPressed() {
    return getStateFlag(DEBOUNCED_STATE) && getStateFlag(CHANGED_STATE);
}

bool Akta3d_TFT_Button::justReleased() {
    return  !getStateFlag(DEBOUNCED_STATE) && getStateFlag(CHANGED_STATE);
}

void Akta3d_TFT_Button::drawButton() {
  // Draw button background
  _tft->fillRoundRect(_x , _y , _w , _h, _radius, _fillColor); 

  // To increase speed, draw border only if it has a different color than button background
  if(_fillColor != _outlineColor) {
    _tft->drawRoundRect(_x , _y , _w , _h, _radius, _outlineColor);
  }

  // Draw text
  _tft->setTextColor(_textColor);
  int16_t X1, Y1; // variables needed to use getTextBounds()
  uint16_t W, H;
  _tft->setFont(NULL);
  _tft->setTextSize(_textSize);
  _tft->getTextBounds(_text, 0, 0, &X1, &Y1, &W, &H);
  uint16_t txtX = _x + (_w - W) / 2;
  uint16_t txtY = _y + (_h - H) / 2;
  _tft->setCursor(txtX , txtY);
  _tft->print(_text);
}

void Akta3d_TFT_Button::updateButtonList(Akta3d_TFT_Button **buttons, Akta3d_Touch_Info info)
{
    for (uint16_t i = 0 ; buttons[i] != NULL; i++) {
        buttons[i]->update(info);
    }
}