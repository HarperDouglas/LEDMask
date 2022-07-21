#include <FastLED.h>
#define NUM_LEDS 45
#define DATA_PIN 6
#define BRIGHTNESS 100 
#define SATURATION 255  
CRGB leds[NUM_LEDS];

/*
row 1: 6-0
row 2: 7-14
row 3: 21-15
row 4: 22-29
row 5: 36-30
row 6: 37-44*/

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(125);
}

void loop() {
  //TODO: Add hardware to be able to swap between modes without redeploying code :P
  rainbow();
}

void reset() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
}

void singleColor(int r, int g, int b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(r, g, b);
  }
  FastLED.show();
  delay(30);
}

void singleColor(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  FastLED.show();
  delay(30);
}

//I found this on stack overflow im lazy
void rainbow() {
  for (int j = 0; j < 255; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(i - (j * 2), SATURATION, BRIGHTNESS);
    }
    FastLED.show();
    delay(25); /*Speed adjust*/
  }
}

//TODO: attach basic mic inside mask to be able to have mask "talk"
void mouthOpen() {
  reset();
  for(int i = 20; i > 15; i--) {
    leds[i] = CRGB::Red;
    leds[i + 15] = CRGB::Red;
  }
  leds[23] = CRGB::Red;
  leds[28] = CRGB::Red;
  FastLED.show();
  delay(30);
}

void mouthClosed() {
  reset();
  for(int i = 23; i <= 28; i++) {
    leds[i] = CRGB::Red;
  }
  FastLED.show();
  delay(30);
}

void smile() { //:)
  reset();
  for(int i = 34; i > 31; i--) {
    leds[i] = CRGB::Red;
  }
  leds[23] = CRGB::Red;
  leds[28] = CRGB::Red;
  FastLED.show();
  delay(30);
}

void fire() {
  //Generating randomized fire
  /*red, orange, white
  row 6 always red
  row 5 has a likely chance to be red, chance to be black, chance to be orange
  if not black, next row up has a chance to be the next color up, same color, or black
  the lower the level the more stable, less often it changes.
  */
  for(int i = 37; i < NUM_LEDS; i++) {
     leds[i] = CRGB::Red;
  }
  /*row 5*/
  int prev = 0;
  int r = random(10);

  for(int i = 36; i >= 30; i--) {
    int c = random(2);
    if(c <= 1)
      r = random(10);
    if(prev >= 3)
      r +=1;
    if(r < 2){
      fireSpread(i, CRGB::Black, 0);
    }
     else if(r < 3) {
      fireSpread(i, CRGB::Orange, 0);
     }
     else 
      fireSpread(i, CRGB::Red, 0);
    prev = r;
  }
  for(int i = 6; i >= 0; i--) {
    int c = random(20);
    if(c == 0)
    leds[i] = CRGB::White;
  }
  FastLED.show();
  reset();
  delay(115);
}

void fireSpread(int i, CRGB::HTMLColorCode color, int mod) {
  int c = random(1);
  int r = random(10) - mod;
  int newNum = i - 7 - c;
  if(newNum < 0)
    return;
  leds[i] = color;
  if(color == CRGB::Red) {
        if(r < 6){
      fireSpread(newNum, CRGB::Black, mod + 1);
    }
     else if(r < 7) {
      fireSpread(newNum, CRGB::Orange, mod + 1);
     }
     else {
      fireSpread(newNum, CRGB::Red, mod + 1);
     }
  }
  else if(color == CRGB::Orange) {
        if(r < 5){
      fireSpread(newNum, CRGB::Black, mod + 1);
    }
     else {
      fireSpread(newNum, CRGB::White, mod - 1);
     }
  }
  else if(color == CRGB::White) {
        if(r < 4){
      fireSpread(newNum, CRGB::Black, mod + 1);
    }
     else {
      fireSpread(newNum, CRGB::White, mod - 1);
     }
  }
}

//TODO: Work in progress. Need to make multiple beams be able to spawn at once, probably requires a total rework or maybe just multithread the arduino gg ez
void rainbowRoad() {
  int spd = random(7) * 20 + 20;
  int row = random(5);
  rainbowBeam(row, spd);
}

void rainbowBeam(int row, int spd) {
  //Coming back a week later to look at this, this system of numbering rows/columns alternating forwards/backwards is absolutely incomprehensible
  int n = 9;
  if(row % 2 == 0) { //even rows
    int p = (row)/2;
    int rowStart = p * 14 + (row - p);
    for(int i = 6 + n + rowStart; i >= rowStart; i--) {
      if(i - n >= rowStart) {
        leds[i - n] = CRGB::White;
      }
      for(int j = 1; j < n; j ++) {
        if(i+j - n < 7 + rowStart && i + j - n >= rowStart) {
        leds[i + j - n] = CHSV(360 - (n - j) * 360/(n-1), SATURATION, BRIGHTNESS);
        }
      }
      FastLED.show();
      reset();
      delay(spd);
    }
    //i could not explain this code if i tried
  } else {
    int p = (row -1)/2;
    int rowStart = p * 14 + (row - p);
    for(int i = 7 + rowStart; i <= 13 + rowStart + n ; i++) {
      if(i <= 13 + rowStart) {
        leds[i] = CRGB::White;
      }
      for(int j = 1; j < n; j ++) {
        if(i + j - n < 14 + rowStart && i + j - n >= 6 + rowStart) {
        leds[i + j - n] = CHSV(360 - (n - j  + 1) * 360/(n-1), SATURATION, BRIGHTNESS);
        }
      }
      FastLED.show();
      reset();
      delay(spd);
    } 
  }
  
  delay(30);
}

//You know exactly what this does
void carameldansen() {
  int numColors = 4;
  CRGB colors[numColors] = {CRGB::Red, CRGB::Green, CRGB:: Blue, CRGB::Purple};
  for(int i = 0; i < numColors; i++) {
    singleColor(colors[i]);
    delay(350);
  }
}
