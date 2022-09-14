/*
(c)saigon 2022  
Written: Sep 12 2022.
Last Updated: Sep 14 2022

Звезда для новогодней елки на WS2812
Пять лучей по 5 светодиодов в каждом

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



switch (1) {
  case 0:

// ------------------------ Искры -----------------

 FastLED.clear();
 FastLED.show();
 delay(200);        // Время паузы
 
 leds[random(LED_NUM)].setRGB(255, 0, 0);
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
  case 2:
  
// ------------------------ Бегущие точки -----------------
  for (int i = 0; i < LED_NUM; i++) {
  FastLED.clear();
  FastLED.show();
    leds[i].setHue(255);
    leds[LED_NUM - i - 1].setHue(255);
  FastLED.show();
  delay(30);        // время свечения точек
  }

    break;
    
}

}
