#include "Akta3d_TFT_Utils.h"


uint16_t Akta3d_TFT_Utils::rgbToHex(RGB rgb) {
  double scaleR = 31/255.0;
  double scaleG = 63/255.0;
  double scaleB = 31/255.0;

  double tmpR = rgb.r * scaleR;
  double tmpG = rgb.g * scaleG;
  double tmpB = rgb.b * scaleB;

  uint16_t res = ((uint16_t)tmpR << 11) | ((uint16_t)tmpG << 5) | (uint16_t)tmpB;
  /*
  Serial.print("R:");
  Serial.print(r);
  Serial.print(", G:");
  Serial.print(g);    
  Serial.print(", B:");
  Serial.print(b);   
  Serial.print(" => r:");
  Serial.print(tmpR); 
  Serial.print(", g:");
  Serial.print(tmpG); 
  Serial.print(", b");
  Serial.print(tmpB);  
  Serial.print(" => r:");
  Serial.println(res);  
  */
  return res;
}


RGB Akta3d_TFT_Utils::hsvToRgb(HSV hsv) {
    // Note that this algorithm uses 0-255 as it's range (not 0-360) 
    hsv = {map(hsv.h, 0 , 359, 0, 255), map(hsv.s, 0 , 100, 0, 255), map(hsv.v, 0 , 100, 0, 255)};

    RGB rgb;
    uint16_t region, remainder, p, q, t;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6; 

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }
    /*
    Serial.print("R:");
    Serial.print(rgb.r);
    Serial.print(", G:");
    Serial.print(rgb.g);    
    Serial.print(", B:");
    Serial.print(rgb.b);   
    Serial.print(" => h:");
    Serial.print(hsv.h); 
    Serial.print(", s:");
    Serial.print(hsv.s); 
    Serial.print(", v");
    Serial.println(hsv.v);
    */
    return rgb;
}

HSV Akta3d_TFT_Utils::rgbToHsv(RGB rgb) {
    // Note that this algorithm uses 0-255 as it's range (not 0-360) 
    HSV hsv;
    uint16_t rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    hsv = {
      map(hsv.h, 0, 255, 0, 359),
      map(hsv.s, 0, 255, 0, 100),
      map(hsv.v, 0, 255, 0, 100),      
    }; 
  
/*
    Serial.print("R:");
    Serial.print(rgb.r);
    Serial.print(", G:");
    Serial.print(rgb.g);    
    Serial.print(", B:");
    Serial.print(rgb.b);   
    Serial.print(" => h:");
    Serial.print(hsv.h); 
    Serial.print(", s:");
    Serial.print(hsv.s); 
    Serial.print(", v");
    Serial.println(hsv.v); 
*/
    return hsv;  
}

uint16_t Akta3d_TFT_Utils::hsvToHex(HSV hsv) {
  RGB rgb = hsvToRgb(hsv);
  return rgbToHex(rgb);
}

HSV Akta3d_TFT_Utils::hexToHsv(uint16_t color) {
  RGB rgb = {
    ((color & 0xF800) >> 8),
    ((color & 0x07E0) >> 3),
    ((color & 0x001F) << 3)
  };
  return rgbToHsv(rgb);
}

// get pressed coordinates
Akta3d_Touch_Info Akta3d_TFT_Utils::getTouchInfo(MCUFRIEND_kbv *tft, TouchScreen *ts, Akta3d_Screen_Settings settings) {
    TSPoint p = ts->getPoint();
    pinMode(settings.yp, OUTPUT);      //restore shared pins
    pinMode(settings.xm, OUTPUT);
    digitalWrite(settings.yp, HIGH);   //because TFT control pins
    digitalWrite(settings.xm, HIGH);
    bool pressed = (p.z > settings.minPressure && p.z < settings.maxPressure);
    int x=0, y=0;

    if (pressed) {
      //Serial.println("pressed");
      if(settings.orientation == PORTRAIT) {      
        x = map(p.x, settings.tsLeft, settings.tsRight, 0, tft->width()); //.kbv makes sense to me
        y = map(p.y, settings.tsTop, settings.tsBottom, 0, tft->height());
      } else {
        x = map(p.y, settings.tsTop, settings.tsBottom, 0, tft->width()); //.kbv makes sense to me
        y = map(p.x, settings.tsRight, settings.tsLeft, 0, tft->height());
      }       
    } 
    return {pressed, x, y};  
}
