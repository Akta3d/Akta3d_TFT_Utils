# Akta3d_TFT_Utils

Arduino TouchScreen helpers and componants  
Initially develop for 240*320 screen  

## Dependencies
- Adafruit_TouchScreen
- Adafruit_GFX_Library
- MCUFRIEND_kbv

## Helpers
### Akta3d_TFT_Colors
- Define global colors

### Akta3d_TFT_Utils
- Static methods to convert colors (RGB, HSV, 565)
- Manage TouchScreen touch event

## Componants
### Akta3d_TFT_Button
- TouchScreen button with debounce
Replace the Adafruit button and is inspired from Bounce2 for the debounce

### Akta3d_TFT_HSlider
- Horizontal slider
- Allow to display or not label with value
- Manage min/max bounds

### Akta3d_TFT_Color_Picker
HSV Color picker
- TODO : Need improvment to draw the Hue/saturation faster (try to use GFXcanvas16)

### Akta3d_TFT_MP3_Control
Componant to control a MP3 Player
- Previous song
- Play/Pause
- Next song
- Loop mode : no loop | loop song | loop all the directory
- Control volume with slider
TODO : Add class to send command to DFRobotDFPlayerMini or to send Serial message to another Arduino board
