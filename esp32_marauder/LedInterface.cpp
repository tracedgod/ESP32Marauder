#include "LedInterface.h"

LedInterface::LedInterface() {}

void LedInterface::RunSetup() {
#ifdef HAS_NEOPIXEL_LED
  //Serial.println("Setting neopixel to black...");
  strip.setBrightness(0);
  strip.begin();
  strip.setPixelColor(0, strip.Color(0, 0, 0));
  strip.show();
  //delay(100);
  strip.setBrightness(50);
  strip.setPixelColor(0, strip.Color(0, 0, 0));
  strip.show();
#elif defined(MARAUDER_FLIPPER)
  pinMode(FLIPPER_LED_R_PIN, OUTPUT);
  pinMode(FLIPPER_LED_G_PIN, OUTPUT);
  pinMode(FLIPPER_LED_B_PIN, OUTPUT);
  digitalWrite(FLIPPER_LED_R_PIN, HIGH);
  digitalWrite(FLIPPER_LED_G_PIN, HIGH);
  digitalWrite(FLIPPER_LED_B_PIN, HIGH);
#elif defined(XIAO_ESP32_S3)
  pinMode(XIAO_LED_PIN, OUTPUT);
  digitalWrite(XIAO_LED_PIN, HIGH);
#elif defined(MARAUDER_M5STICKC)
  pinMode(STICKC_LED_PIN, OUTPUT);
  digitalWrite(STICKC_LED_PIN, HIGH);
#endif

  this->initTime = millis();
}

void LedInterface::main(uint32_t currentTime) {
  if ((!settings_obj.loadSetting<bool>("EnableLED")) ||
      (this->current_mode == LedMode::OFF)) {
    this->ledOff();
    return;
  }

  switch (this->current_mode) {
#ifdef HAS_NEOPIXEL_LED
    case LedMode::RAINBOW:
      this->rainbow();
      break;
#endif
    case LedMode::ATTACK:
      this->attackLed();
      break;
    case LedMode::SNIFF:
      this->sniffLed();
      break;
    case LedMode::CUSTOM:
      // placeholder for custom led actions
      break;
    default:
      this->ledOff();
      break;
  }
};

void LedInterface::setMode(LedMode new_mode) {
  this->current_mode = new_mode;
}

LedMode LedInterface::getMode() {
  return this->current_mode;
}

void LedInterface::setColor(int r, int g, int b) {
#ifdef HAS_NEOPIXEL_LED
  strip.setPixelColor(0, strip.Color(r, g, b));
  strip.show();
#elif defined(MARAUDER_FLIPPER)
  digitalWrite(FLIPPER_LED_R_PIN, r > 0 ? LOW : HIGH);
  digitalWrite(FLIPPER_LED_G_PIN, g > 0 ? LOW : HIGH);
  digitalWrite(FLIPPER_LED_B_PIN, b > 0 ? LOW : HIGH);
#endif
}

void LedInterface::sniffLed() {
#ifdef XIAO_ESP32_S3
  digitalWrite(XIAO_LED_PIN, LOW);
  delay(300);
  digitalWrite(XIAO_LED_PIN, HIGH);
#elif defined(MARAUDER_M5STICKC)
  digitalWrite(STICKC_LED_PIN, LOW);
  delay(300);
  digitalWrite(STICKC_LED_PIN, HIGH);
#else
  this->setColor(0, 0, 255);
#endif
}

void LedInterface::attackLed() {
#ifdef XIAO_ESP32_S3
  digitalWrite(XIAO_LED_PIN, LOW);
  delay(300);
  digitalWrite(XIAO_LED_PIN, HIGH);
#elif defined(MARAUDER_M5STICKC)
  digitalWrite(STICKC_LED_PIN, LOW);
  delay(300);
  digitalWrite(STICKC_LED_PIN, HIGH);
#else
  this->setColor(255, 0, 0);
#endif
}

void LedInterface::ledOff() {
#ifdef XIAO_ESP32_S3
  digitalWrite(XIAO_LED_PIN, HIGH);
#elif defined(MARAUDER_M5STICKC)
  digitalWrite(STICKC_LED_PIN, HIGH);
#else
  this->setColor(0, 0, 0);
#endif
}

#ifdef HAS_NEOPIXEL_LED
void LedInterface::rainbow() {
  strip.setPixelColor(0, this->Wheel((0 * 256 / 100 + this->wheel_pos) % 256));
  strip.show();

  this->current_fade_itter++;

  this->wheel_pos = this->wheel_pos - this->wheel_speed;
  if (this->wheel_pos < 0)
    this->wheel_pos = 255;
}

uint32_t LedInterface::Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
#endif