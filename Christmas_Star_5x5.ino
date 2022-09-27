/*
(c)saigon 2022  
Written: Sep 12 2022.
Last Updated: Sep 27 2022

Звезда для новогодней елки на WS2812
Пять лучей по 5 светодиодов в каждом
Девять эффектов в двух группах, пять основных и четыре дополнительных
Четыре режима включения эффектов:
  0- по порядку из списка избранных эфектов
  1- случайно из списка избранных эффектов
  2- по порядку, чередуя основной эффект с дополнительным
  3- в случайном порядке, чередуя основной эффект с дополнительным
Время свечения основных и дополнителльных эффектов устанавливается отдельно
Перечень эффектов:
основные:
  0 - Искры
  1 - Дыхание
  2 - Блеск
  3 - Сердцебиение
  4 - Свечение с искрами
дополнительные:
  5 - Бегущие точки
  6 - Водопад
  7 - Змейка
  8 - Мигающие лучи
*/

#define LED_PIN 2                 // пин ленты
#define LED_NUM 25                // количество светодиодов
#define MAX_BRIGHTNESS 150        // максимальная яркость ленты

#include "FastLED.h"
CRGB leds[LED_NUM];
unsigned long paletteTimer;       // таймер показа эффектов

byte paletteMode = 1;             // режим включения эффектов
byte currentPalette = 0;          // текущая палитра
byte currentBasicPalette = 0;     // текущая основная палитра
byte currentShortPalette = 5;     // текущая дополнительная палитра
byte basicPaletteInterval = 10;   // продолжительность основных (1,2,3,4,5) эффектов (сек) 
byte shortPaletteInterval = 5;    // продолжительность дополнительных (6,7,8,9) эффектов (сек)

byte favoritePalette[] = {0,1,2,3,4,5,6,7,8}; // список избранных эффектов
                                              // дублирование эффекта в списке, увеличивает частоту его появления
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

// матрица 11х4 сверху слева вниз направо
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
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  randomSeed(analogRead(0));
  paletteTimer = millis();
}

void loop() {

switch (paletteMode) { // реализация режимов свечения
//switch (7) {  
  case 0:  // по порядку из списка избранных эфектов
if (currentPalette > 4 & millis() >= paletteTimer + shortPaletteInterval * 1000) {
  currentPalette=random(sizeof(favoritePalette)-4);
  paletteTimer = millis(); 
 } 
  else { 
  if (millis() >= paletteTimer + basicPaletteInterval * 1000) {
  currentPalette=random(5, sizeof(favoritePalette));
  paletteTimer = millis(); 
  }
}
    break;
 
  
  case 1:  // случайно из списка избранных эффектов
if (currentPalette > 4 & millis() >= paletteTimer + shortPaletteInterval * 1000) {
  currentPalette=random(sizeof(favoritePalette)-4);
  paletteTimer = millis(); 
 } 
  else { 
  if (millis() >= paletteTimer + basicPaletteInterval * 1000) {
  currentPalette=random(5, sizeof(favoritePalette));
  paletteTimer = millis(); 
  }
}
    break;

  
  case 2:  // по порядку, чередуя основной эффект с дополнительным

    break;
  
  
  case 3:  // в случайном порядке, чередуя основной эффект с дополнительным
if (currentPalette > 4 & millis() >= paletteTimer + shortPaletteInterval * 1000) {
  currentBasicPalette++;
   if (currentBasicPalette >4) currentBasicPalette = 0; 
      currentPalette=currentBasicPalette;
  paletteTimer = millis(); 
 } 
  else { 
  if (millis() >= paletteTimer + basicPaletteInterval * 1000) {
    currentShortPalette++;
    if (currentShortPalette >8) currentShortPalette = 5; 
  currentPalette=currentShortPalette;
  paletteTimer = millis(); 
  }
}
    break;
 }


switch (currentPalette) {
//switch (7) {  
  case 0:
  sparks();         // Искры
    break;
  case 1:
  breathe();        // Дыхание
    break;
  case 2:
   gloss();         // Блеск
    break;
  case 3:
  heartBeat();      // Сердцебиение
    break;
  case 4:
  sparkle();        // Свечение с искрами
    break;
  case 5:  
  runningDots();    // Бегущие точки
    break;
  case 6:  
  waterfall();      // Водопад
    break;
  case 7:  
  snake();          // Змейка
    break;
  case 8:  
  flashingRays();   // Мигающие лучи
    break;
 }
}

// ============================= ЭФФЕКТЫ ==============================

void sparks(){        // ------------------------ Искры -----------------
 FastLED.clear();
 FastLED.show();
 delay(100);        // Время паузы
 leds[random(LED_NUM)].setHue(255); // две искры одновременно рандомно
 leds[random(LED_NUM)].setHue(255);
  FastLED.show();
  delay(25);        // Время вспышки
}

void breathe(){       // ------------------------ Дыхание -----------------
int min_brightness = 80; // минимальная яркость (ниже 10 - некрасиво мерцает)
int q;
  for (int j = min_brightness; j < 512 - min_brightness; j++) {
    if (j > 255) q=511-j; else q=j;
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHSV(255, 255, q);
  }
  // leds[random(LED_NUM)].setHSV(255, 255, 255);   // искры
  FastLED.show();
  delay(7);       // скорость дыхания
 }
}

void gloss(){         // ------------------------ Блеск ----------------- 
  FastLED.clear();
  FastLED.show();
  for (int i = 0;  i < 11; i++) {
   for (int g = 0; g < LED_NUM; g++) {
    leds[g].setHSV(255, 255, 155);
  } 
    for (int j = 0;  j < 4; j++) {
    leds[glossMatrix2[i][j]].setHSV(255, 255, 255); // порядок влючения точек из матрицы
  }
  FastLED.show();
  delay(30);
 }
  leds[glossMatrix2[10][0]].setHSV(255, 255, 155);
  FastLED.show();
  delay(1000+random(21)*100); // рандомно блеск от 1 до 3 сек
}

void heartBeat(){     // ------------------------ Сердцебиение -----------------
int min_brightness = 60; // минимальная яркость (ниже 10 - некрасиво мерцает)
int q;
  for (int j = min_brightness; j < 512 - min_brightness; j+=2) {
    if (j > 255) q=511-j; else q=j;
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHSV(255, 255, q);
  }
 FastLED.show();
 if (j > (255-min_brightness)/3 & j < (255 + (255-min_brightness)/3))
  delay(2);      // скорость яркой вспышки 
  else
   delay(14);    // скорость затухания
 }
}

void sparkle(){       // ------------------------ Свечение с искрами -----------------
int background_brightness = 100; // яркость фона
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHSV(255, 255, background_brightness);
  }
 leds[random(LED_NUM)].setHSV(255, 255, 255);   // искры
 FastLED.show();
 delay(10);
}

void runningDots(){   // ------------------------ Бегущие точки -----------------
 for (int i = 0; i < LED_NUM; i++) {
  FastLED.clear();
  FastLED.show();

   leds[i].setHue(255);    
   leds[LED_NUM-i-1].setHue(255);
  FastLED.show();
  delay(30);        // время свечения точек
 }
}

void waterfall(){     // ------------------------ Водопад ----------------- 
  for (int i = 0;  i < 11; i++) {
  FastLED.clear();
  FastLED.show();
   for (int j = 0;  j < 4; j++) {
    leds[glossMatrix[10 - i][j]].setHue(255); // порядок влючения точек из матрицы
  }
  FastLED.show();
  delay(50);
 } 
}

void snake(){         // ------------------------ Змейка -----------------
for (int j = 0;  j < LED_NUM; j++) {
  for (int i = 0; i < LED_NUM; i++) {
   leds[i].setHSV(255, 255, (i-j)*255/LED_NUM);
  }
  FastLED.show();
  delay(40);  // скорость змейки
 } 
}

void flashingRays(){  // ------------------------ Мигающие лучи -----------------
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
