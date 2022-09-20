/*
(c)saigon 2022  
Written: Sep 12 2022.
Last Updated: Sep 20 2022

Звезда для новогодней елки на WS2812
Пять лучей по 5 светодиодов в каждом

*/

#define LED_PIN 2               // пин ленты
#define LED_NUM 25              // количество светодиодов
#define COLON_SIZE 4            // высота матрицы
#define COLON_AMOUNT 5          // ширина матрицы
#define MIN_BRIGHTNESS 0        // минимальная яркость ленты. Если больше нуля, звезда не будет гаснуть полностью
#define MAX_BRIGHTNESS 50       // максимальная яркость ленты

#include "FastLED.h"
CRGB leds[LED_NUM];
unsigned long paletteTimer = 0; // таймер показа эффектов
int showPaletteInterval = 10;   // продолжительность показа выбранного эффекта (сек)
byte currentPalette=0;          // текущая палитра
byte counter= MAX_BRIGHTNESS-1;
boolean dir=true; 
/*
// матрица 11х4
byte glossMatrix[][4] = {
  {7,12,12,12},
  {8,11,11,11},
  {6,9,10,13},
  {5,14,14,14},
  {4,15,15,15},
  {3,16,16,16},
  {2,17,17,17},
  {0,1,18,19},
  {20,24,24,24},
  {21,23,23,23},
  {22,22,22,22},
};
*/

// матрица 11х4
byte glossMatrix[][4] = {
  {2,22,22,22},
  {1,23,23,23},
  {3,0,24,21},
  {4,20,20,20},
  {5,19,19,19},
  {6,18,18,18},
  {7,8,16,17},
  {9,15,15,15},
  {10,14,14,14},
  {11,13,13,13},  
  {12,12,12,12},
};

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  randomSeed(analogRead(0));
  paletteTimer = millis();
}

void loop() {
/*
if (millis() >= paletteTimer + showPaletteInterval * 1000) {
 currentPalette++;
 if (currentPalette >4) currentPalette = 0; 
 paletteTimer = millis(); 
}

switch (currentPalette) {
*/  

switch (1) {  
  case 0:
  sparks();
    break;
  case 1:
  breathe();
    break;
  case 2:
  runningDots();
    break;
  case 3:
  gloss();
    break;
  case 4:  
  heartBeat2();
    break;
}
}
// ----------- END LOOP --------------------


// ======================= ЭФФЕКТЫ =========================

void sparks(){      // ------------------------ Искры -----------------
 FastLED.clear();
 FastLED.show();
 delay(100);        // Время паузы
 
 leds[random(LED_NUM)].setHue(255);
 leds[random(LED_NUM)].setHue(255);
  FastLED.show();
  delay(25);        // Время вспышки
}

void breathe(){      // ------------------------ Дыхание -----------------
int q;
  for (int j = 30; j < 482; j++) {
    if (j > 255) q=511-j; else q=j;
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHSV(255, 255, q);
  }
  FastLED.show();
  delay(5);
}
}

void runningDots(){    // ------------------------ Бегущие точки -----------------
  for (int i = 0; i < LED_NUM; i++) {
  FastLED.clear();
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.show();
    leds[i].setHue(255);
    leds[LED_NUM-i-1].setHue(255);
  FastLED.show();
  delay(30);        // время свечения точек
  }
}

void gloss(){      // ------------------------ Блеск ----------------- 
  for (int i = 0;  i < 11; i++) {
  FastLED.clear();
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.show();
    
    for (int j = 0;  j < 4; j++) {
//     Serial.println(glossMatrix[i][j]);
//    leds[glossMatrix[i][j]].setHue(255);
    leds[glossMatrix[10 - i][j]].setHue(255);
  }
  FastLED.show();
  delay(30);
 } 
}

void heartBeat(){      // ------------------------ Сердцебиение -----------------
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHue(255);
  }
  FastLED.setBrightness(counter);
  FastLED.show();
    if (dir) counter++; // увеличиваем яркость
    else counter--;     // уменьшаем
    if (counter >= MAX_BRIGHTNESS || counter <= MAX_BRIGHTNESS/3) dir = !dir;   // разворачиваем
  if (counter >= MAX_BRIGHTNESS/3 & counter <= MAX_BRIGHTNESS - MAX_BRIGHTNESS/3*2 ) delay(50);    // скорость дыхания
  else   delay(15);
}

void heartBeat2(){      // ------------------------ Сердцебиение -----------------
    for (int j = 0;  j < 25; j++) {
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHSV(255, 255, (i-j+50) * 10);
  }
  FastLED.show();
  delay(30);
} 
}
