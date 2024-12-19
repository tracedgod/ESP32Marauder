#pragma once

#ifndef LedInterface_h
#define LedInterface_h

#include "configs.h"
#include "settings.h"

#include <Arduino.h>

#ifdef HAS_NEOPIXEL_LED
#include <Adafruit_NeoPixel.h>
#endif

enum class LedMode : uint8_t {
  OFF = 0,
  RAINBOW = 1,
  ATTACK = 2,
  SNIFF = 3,
  CUSTOM = 4
};

extern Settings settings_obj;

#ifdef HAS_NEOPIXEL_LED
extern Adafruit_NeoPixel strip;
#endif

class LedInterface {

  private:
    uint32_t initTime = 0;

#ifdef HAS_NEOPIXEL_LED
    int current_fade_itter = 1;
    int wheel_pos = 255;
    int wheel_speed = 1; // lower = slower

    uint32_t Wheel(byte WheelPos);

    void rainbow();
#endif

    LedMode current_mode = LedMode::OFF;

    void ledOff();
    void attackLed();
    void sniffLed();
  
  public:
    LedInterface();

    void RunSetup();
    void main(uint32_t currentTime);

    void setMode(LedMode mode);
    void setColor(int r, int g, int b);
    LedMode getMode();
    
  
};

#endif
