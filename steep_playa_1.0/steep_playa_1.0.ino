
// Jmann


#include <FastLED.h>
#define LED_PIN 7
#define COLOR_ORDER BRG
#define LED_TYPE WS2812B
#define NUM_LEDS 150
#define BRIGHTNESS 120

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
    delay(2000);
    LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER> (leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  // put your main code here, to run repeatedly:
leds[0] = CRGB::HotPink;
leds[2] = CRGB::HotPink;
FastLED.show();
delay(8000);

  fill_solid(leds, NUM_LEDS, 0xFF61A5);
  FastLED.show();
  delay(2000);
  FastLED.clear();

  fill_solid(leds, NUM_LEDS, 0x3CFF57);
  FastLED.show();
  delay(2000);
  FastLED.clear();

  fill_solid(leds, NUM_LEDS, 0xFFDA54);
  FastLED.show();
  delay(3000);
  FastLED.clear();

FastLED.clear();

 for(int i=0;i<NUM_LEDS;i++){  
   leds[i-2] = 0xFF61A5;
   leds[i-1] = 0x3CFF57;
   leds[i] = 0xFFDA54;
   FastLED.show();
   delay(50);
   FastLED.clear();
 }
 for(int i=0;i<NUM_LEDS;i++){
   fill_rainbow(leds,i, 0, 5);
   FastLED.show();
   delay(20);
 }
  FastLED.clear();
}
