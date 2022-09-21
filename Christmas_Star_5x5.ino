/*
(c)saigon 2022  
Written: Sep 12 2022.
Last Updated: Sep 21 2022

Звезда для новогодней елки на WS2812
Пять лучей по 5 светодиодов в каждом

*/

#define LED_PIN 2               // пин ленты
#define LED_NUM 25              // количество светодиодов
#define COLON_SIZE 4            // высота матрицы
#define COLON_AMOUNT 5          // ширина матрицы
#define MIN_BRIGHTNESS 0        // минимальная яркость ленты. Если больше нуля, звезда не будет гаснуть полностью
#define MAX_BRIGHTNESS 150       // максимальная яркость ленты

#include "FastLED.h"
CRGB leds[LED_NUM];
unsigned long paletteTimer; // таймер показа эффектов
int showPaletteInterval = 10;   // продолжительность показа выбранного эффекта (сек)
byte currentPalette=0;          // текущая палитра
byte counter= MAX_BRIGHTNESS-1;
boolean dir=true; 

// матрица 11х4 сверху вниз
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

// матрица 11х4 сверху слева вверх направо
byte glossMatrix2[][4] = {
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

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  randomSeed(analogRead(0));
  paletteTimer = millis();
}

void loop() {
  
if (millis() >= paletteTimer + showPaletteInterval * 1000) {
 currentPalette++;
 if (currentPalette >8) currentPalette = 0; 
 paletteTimer = millis(); 
}

switch (currentPalette) {  

//switch (4) {  
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
  waterfall();
    break;
  case 4:
  gloss();
    break;
  case 5:  
  heartBeat();
    break;
  case 6:  
  snake();
    break;
  case 7:  
  sparkle();
    break;
  case 8:  
  flashingRays();
    break;
}
}
// ----------- END LOOP --------------------


// ======================= ЭФФЕКТЫ =========================

void sparks(){      // ------------------------ Искры -----------------
 FastLED.clear();
 FastLED.show();
 delay(100);        // Время паузы
 
 leds[random(LED_NUM)].setHue(255); // две искры одновременно рандомно
 leds[random(LED_NUM)].setHue(255);
  FastLED.show();
  delay(25);        // Время вспышки
}

void breathe(){      // ------------------------ Дыхание -----------------
int min_brightness = 60; // минимальная яркость (ниже 10 - некрасиво мерцает)
int q;
  for (int j = min_brightness; j < 512 - min_brightness; j++) {
    if (j > 255) q=511-j; else q=j;
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHSV(255, 255, q);
  }
 // leds[random(LED_NUM)].setHSV(255, 255, 255);   // блёстки
 FastLED.show();
  delay(7);       // частота дыхания
}
}

void runningDots(){    // ------------------------ Бегущие точки -----------------
 for (int i = 0; i < LED_NUM; i++) {
  FastLED.clear();
  FastLED.show();

   leds[i].setHue(255);    
   leds[LED_NUM-i-1].setHue(255);
  FastLED.show();
  delay(30);        // время свечения точек
  }
}


void waterfall(){      // ------------------------ Водопад ----------------- 
  for (int i = 0;  i < 11; i++) {
  FastLED.clear();
  FastLED.show();
    
    for (int j = 0;  j < 4; j++) {
//     Serial.println(glossMatrix[i][j]);
//    leds[glossMatrix[i][j]].setHue(255);
    leds[glossMatrix[10 - i][j]].setHue(255);
  }
  FastLED.show();
  delay(50);
 } 
}

void gloss(){      // ------------------------ Блеск ----------------- 
  FastLED.clear();
  FastLED.show();
  for (int i = 0;  i < 11; i++) {
   for (int g = 0; g < LED_NUM; g++) {
    leds[g].setHSV(255, 255, 155);
  }   
    for (int j = 0;  j < 4; j++) {
    leds[glossMatrix2[i][j]].setHSV(255, 255, 255);
  }
  FastLED.show();
  delay(30);
 }
  delay(1000+random(20)*100);
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

void snake(){      // ------------------------ Змейка -----------------
    for (int j = 0;  j < 25; j++) {
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHSV(255, 255, (i-j+50) * 10);
  }
  FastLED.show();
  delay(30);
 } 
}

void sparkle(){      // ------------------------ Свечение с искрами -----------------
int background_brightness = 100; // яркость фона
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHSV(255, 255, background_brightness);
  }
 leds[random(LED_NUM)].setHSV(255, 255, 255);   // блёстки
 FastLED.show();
  delay(10);
}

void flashingRays(){    // ------------------------ Мигающие лучи -----------------
int q;
int vals[] = {0, 10, 20, 5, 15}; // порядок переключения лучей

  for (int val : vals) {
  FastLED.clear();
  FastLED.show();

  for (int i = 10; i < 512; i+=3) {
    if (i > 255) q=511-i; else q=i;

  for (int j = 0; j < 5; j++) {
    leds[val+j].setHSV(255, 255, q);
  }
    FastLED.show();
  }
 } 
}
