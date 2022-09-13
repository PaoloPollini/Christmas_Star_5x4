/*
(c)saigon 2022  
Written: Sep 12 2022.
Last Updated: Sep 13 2022

Звезда для новогодней елки на WS2812
Пять лучей по 4 светодиода в каждом

*/

#define LED_PIN 2            // пин ленты
#define LED_NUM 20           // количество светодиодов
#define COLON_SIZE 4         // высота матрицы
#define COLON_AMOUNT 5       // ширина матрицы
#define MAX_BRIGHTNESS 50   // максимальная яркость ленты

#include "FastLED.h"
CRGB leds[LED_NUM];


void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  randomSeed(analogRead(0));
}

byte counter;
boolean dir=true; 
void loop() {



switch (0) {
  case 0:

// ------------------------ Искры -----------------

 FastLED.clear();
 FastLED.show();
 delay(200);        // Время паузы
 
 leds[random(LED_NUM)].setRGB(255, 255, 255);
  FastLED.show();
  delay(25);        // Время вспышки
  
    break;
  case 1:
  
// ------------------------ Дыхание -----------------
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHue(255);
  }

  FastLED.setBrightness(counter);
  FastLED.show();
    if (dir) counter++; // увеличиваем яркость
    else counter--;     // уменьшаем
    if (counter >= MAX_BRIGHTNESS || counter <= 0) dir = !dir; // разворачиваем
  delay(25);        // скорость дыхания

    break;
}

}
void sparkles() {
  

// ------------------------ Искры -----------------

 FastLED.clear();
 FastLED.show();
 delay(200);        // Время паузы
 
 leds[random(LED_NUM)].setRGB(255, 255, 255);
  FastLED.show();
  delay(25);        // Время вспышки
  
}


void breathe() {

// ------------------------ Дыхание -----------------
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHue(255);
  }

  FastLED.setBrightness(counter);
  FastLED.show();
    if (dir) counter++; // увеличиваем яркость
    else counter--;     // уменьшаем
    if (counter >= MAX_BRIGHTNESS || counter <= 0) dir = !dir; // разворачиваем
  delay(25);        // скорость дыхания
}
