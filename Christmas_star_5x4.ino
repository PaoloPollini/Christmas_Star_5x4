/*
(c)saigon 2022  
Written: Sep 12 2022.
Last Updated: Sep 12 2022

Звезда для новогодней елки на WS2812
Пять лучей по 4 светодиода в каждом

*/



#define LED_PIN 5
#define LED_NUM 50
#include "FastLED.h"
CRGB leds[LED_NUM];
void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(50);
  
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHue(i * 255 / LED_NUM);
  }
  FastLED.show();
}
void loop(){}
