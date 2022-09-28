/*
  (c)saigon 2022
  Written: Sep 12 2022.
  Last Updated: Sep 28 2022
  GitHub: https://github.com/PaoloPollini/Christmas_Star_5x5

  Звезда для новогодней елки на WS2812 v1.0
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

//#define DEBUG_ENABLE
#ifdef DEBUG_ENABLE
#define DEBUG(x) Serial.println(x)
#else
#define DEBUG(x)
#endif

#define LED_PIN 2                 // пин ленты
#define LED_NUM 25                // количество светодиодов
#define MAX_BRIGHTNESS 150        // максимальная яркость ленты
#define PALETTE_MODE 2            // режим включения эффектов (0-3)
long basicPaletteInterval = 60;   // продолжительность основных (1,2,3,4,5) эффектов (сек) 
long shortPaletteInterval = 20;   // продолжительность дополнительных (6,7,8,9) эффектов (сек)

#include "FastLED.h"
CRGB leds[LED_NUM];

byte counter = 0;                 // счетчик
byte currentPalette = 0;          // текущая палитра
byte currentBasicPalette = 0;     // текущая основная палитра
byte currentShortPalette = 5;     // текущая дополнительная палитра
unsigned long paletteTimer = 0;   // таймер показа эффектов

byte favoritePalette[] = {1,2,3,4,5,6,7,8}; // список избранных эффектов
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
#ifdef DEBUG_ENABLE
  Serial.begin(9600);
#endif
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  randomSeed(analogRead(0));
  paletteTimer = millis();
}

void loop() {
  DEBUG(currentPalette);

// реализация режимов свечения
#if (PALETTE_MODE == 0) // по порядку из списка избранных эфектов
 if (millis() < shortPaletteInterval * 1000 & paletteTimer == 0) { // выполняем один раз при старте
  currentPalette = favoritePalette[0];
  paletteTimer = 1;
 }
  if (currentPalette > 4 & millis() >= paletteTimer + shortPaletteInterval * 1000) {
   counter++;
  if (counter > sizeof(favoritePalette) - 1) counter = 0;
  currentPalette = favoritePalette[counter];
   DEBUG(sizeof(favoritePalette));
   DEBUG(currentPalette);
  paletteTimer = millis();
} else {
  if (millis() >= paletteTimer + basicPaletteInterval * 1000) {
   counter++;
   if (counter > sizeof(favoritePalette) - 1) counter = 0;
    currentPalette = favoritePalette[counter];
     DEBUG(sizeof(favoritePalette));
     DEBUG(currentPalette);
   paletteTimer = millis();
 }
}
#endif

#if (PALETTE_MODE == 1)  // случайно из списка избранных эффектов
 if (millis() < shortPaletteInterval * 1000 & paletteTimer == 0) { // выполняем один раз при старте
  currentPalette = favoritePalette[random(sizeof(favoritePalette))];
  paletteTimer = 1;
 }
  if (currentPalette > 4 & millis() >= paletteTimer + shortPaletteInterval * 1000) {
   currentPalette = favoritePalette[random(sizeof(favoritePalette))];
     DEBUG(sizeof(favoritePalette));
     DEBUG(currentPalette);
   paletteTimer = millis();
} else {
  if (millis() >= paletteTimer + basicPaletteInterval * 1000) {
    currentPalette = favoritePalette[random(sizeof(favoritePalette))];
     DEBUG(sizeof(favoritePalette));
     DEBUG(currentPalette);
    paletteTimer = millis();
  }
 }
#endif

#if (PALETTE_MODE == 2)   // по порядку, чередуя основной эффект с дополнительным
 if (currentPalette > 4 & millis() >= paletteTimer + shortPaletteInterval * 1000) {
   currentBasicPalette++;
   if (currentBasicPalette > 4) currentBasicPalette = 0;
    currentPalette = currentBasicPalette;
     DEBUG(currentPalette);
    paletteTimer = millis();
 } else {
  if (millis() >= paletteTimer + basicPaletteInterval * 1000) {
    currentShortPalette++;
    if (currentShortPalette > 8) currentShortPalette = 5;
    currentPalette = currentShortPalette;
      DEBUG(currentPalette);
    paletteTimer = millis();
  }
 }
#endif

#if (PALETTE_MODE == 3)   // в случайном порядке, чередуя основной эффект с дополнительным
 if (millis() < shortPaletteInterval * 1000) & paletteTimer == 0) { // выполняем один раз при старте
  currentPalette = random(9);
  paletteTimer = 1; 
 }
  if (currentPalette > 4 & millis() >= paletteTimer + shortPaletteInterval * 1000) {
   currentPalette = random(9);
     DEBUG(currentPalette);
   paletteTimer = millis();
 } else {
   if (millis() >= paletteTimer + basicPaletteInterval * 1000) {
   currentPalette = random(9);
     DEBUG(currentPalette);
   paletteTimer = millis();
  }
 }
#endif

  // выбор эффекта
  switch (currentPalette) {
    case 0:  sparks();        break;  // Искры
    case 1:  breathe();       break;  // Дыхание
    case 2:  gloss();         break;  // Блеск
    case 3:  heartBeat();     break;  // Сердцебиение
    case 4:  sparkle();       break;  // Свечение с искрами
    case 5:  runningDots();   break;  // Бегущие точки
    case 6:  waterfall();     break;  // Водопад
    case 7:  snake();         break;  // Змейка
    case 8:  flashingRays();  break;  // Мигающие лучи
  }
}

// ============================= ЭФФЕКТЫ ==============================

void sparks() {       // ------------------------ Искры -----------------
  FastLED.clear();
  FastLED.show();
  delay(100);        // Время паузы
  leds[random(LED_NUM)].setHue(255); // две искры одновременно рандомно
  leds[random(LED_NUM)].setHue(255);
  FastLED.show();
  delay(25);        // Время вспышки
}

void breathe() {      // ------------------------ Дыхание -----------------
  int min_brightness = 80; // минимальная яркость (ниже 10 - некрасиво мерцает)
  int q;
  for (int j = min_brightness; j < 512 - min_brightness; j++) {
    if (j > 255) q = 511 - j; else q = j;
    for (int i = 0; i < LED_NUM; i++) {
      leds[i].setHSV(255, 255, q);
    }
    // leds[random(LED_NUM)].setHSV(255, 255, 255);   // искры
    FastLED.show();
    delay(7);       // скорость дыхания
  }
}

void gloss() {        // ------------------------ Блеск -----------------
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
  delay(1000 + random(21) * 100); // рандомно блеск от 1 до 3 сек
}

void heartBeat() {    // ------------------------ Сердцебиение -----------------
  int min_brightness = 60; // минимальная яркость (ниже 10 - некрасиво мерцает)
  int q;
  for (int j = min_brightness; j < 512 - min_brightness; j += 2) {
    if (j > 255) q = 511 - j; else q = j;
    for (int i = 0; i < LED_NUM; i++) {
      leds[i].setHSV(255, 255, q);
    }
    FastLED.show();
    if (j > (255 - min_brightness) / 3 & j < (255 + (255 - min_brightness) / 3))
      delay(2);      // скорость яркой вспышки
    else
      delay(14);    // скорость затухания
  }
}

void sparkle() {      // ------------------------ Свечение с искрами -----------------
  int background_brightness = 100; // яркость фона
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHSV(255, 255, background_brightness);
  }
  leds[random(LED_NUM)].setHSV(255, 255, 255);   // искры
  FastLED.show();
  delay(10);
}

void runningDots() {  // ------------------------ Бегущие точки -----------------
  for (int i = 0; i < LED_NUM; i++) {
    FastLED.clear();
    FastLED.show();

    leds[i].setHue(255);
    leds[LED_NUM - i - 1].setHue(255);
    FastLED.show();
    delay(30);        // время свечения точек
  }
}

void waterfall() {    // ------------------------ Водопад -----------------
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

void snake() {        // ------------------------ Змейка -----------------
  for (int j = 0;  j < LED_NUM; j++) {
    for (int i = 0; i < LED_NUM; i++) {
      leds[i].setHSV(255, 255, (i - j) * 255 / LED_NUM);
    }
    FastLED.show();
    delay(40);  // скорость змейки
  }
}

void flashingRays() { // ------------------------ Мигающие лучи -----------------
  int q;
  int vals[] = {0, 10, 20, 5, 15}; // порядок переключения лучей

  for (int val : vals) {
    FastLED.clear();
    FastLED.show();
    for (int i = 10; i < 512; i += 3) {
      if (i > 255) q = 511 - i; else q = i;
      for (int j = 0; j < 5; j++) {
        leds[val + j].setHSV(255, 255, q);
      }
      FastLED.show();
    }
  }
}
