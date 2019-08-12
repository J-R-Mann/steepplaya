
// Jmann
//08.11.2019


#include <FastLED.h>
#define LED_PIN 7
#define COLOR_ORDER BRG
#define LED_TYPE WS2812B
#define NUM_LEDS 150
#define BRIGHTNESS 60

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

void setup() {
  // put your setup code here, to run once:
    delay(3000);
    LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER> (leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);   

    currentPalette = RainbowColors_p;
    currentBlending = NOBLEND;
}

// Gradient palette "bhw2_sunsetx_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_sunsetx.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_sunsetx_gp ) {
    0,  42, 55,255,
   25,  73,101,242,
   89, 115,162,228,
  107, 115,162,228,
  114, 100, 77,201,
  127,  86, 23,174,
  142, 190, 32, 24,
  171, 210,107, 42,
  255, 232,229, 67};

CRGBPalette16 myPal = bhw2_sunsetx_gp;

void loop() {
  // put your main code here, to run repeatedly:
leds[0] = CRGB::HotPink;
leds[2] = CRGB::HotPink;
FastLED.show();
delay(100);

  fill_solid(leds, NUM_LEDS, 0xFF61A5);
  FastLED.show();
  delay(100);
  FastLED.clear();

  fill_solid(leds, NUM_LEDS, 0x3CFF57);
  FastLED.show();
  delay(100);
  FastLED.clear();

  fill_solid(leds, NUM_LEDS, 0xFFDA54);
  FastLED.show();
  delay(100);
  FastLED.clear();

FastLED.clear();

 for(int i=0;i<NUM_LEDS;i++){  
   leds[i-2] = 0xFF61A5;
   leds[i-1] = 0x3CFF57;
   leds[i] = 0xFFDA54;
   FastLED.show();
   delay(20);
   
   FastLED.clear();
 }
 for(int i=0;i<NUM_LEDS;i++){
   fill_rainbow(leds,i, 0, 5);
   FastLED.show();
   delay(20);
 }
 
  FastLED.clear();

 for(int i=0;i<NUM_LEDS;i++){
   fill_palette(leds,i, 0, 5,currentPalette,255,currentBlending);
   FastLED.show();
   delay(20);
 }

 
//  for(int i=0;i<255;i++){
//    fill_solid(leds, NUM_LEDS,ColorFromPalette(currentPalette, i));
//    delay(10);
//    }
// FastLED.clear();
// CRGB color = ColorFromPalette( currentPalette, colorindex);
// fill_solid(leds, NUM_LEDS,ColorFromPalette(currentPalette, color));
// delay(3000); 
// FastLED.clear();
// fill_solid(leds, NUM_LEDS,ColorFromPalette(currentPalette, color));
// delay(3000);

//
//  for( int colorStep=0; colorStep<256; colorStep++ ) {
//
//      int r = colorStep;  // Redness starts at zero and goes up to full
//      int b = 255-colorStep;  // Blue starts at full and goes down to zero
//      int g = 0;              // No green needed to go from blue to red
//
//      // Now loop though each of the LEDs and set each one to the current color
//
//      for(int x = 0; x < NUM_LEDS; x++){
//          leds[x] = CRGB(r,g,b);
//      }
//
//
//
//      // Display the colors we just set on the actual LEDs
//      FastLED.show();
//
//      delay(10); 
//  }


  for( int colorStep=0; colorStep<240; colorStep++ ) {
      int color = colorStep;
      // Now loop though each of the LEDs and set each one to the current color

      for(int x = 0; x < NUM_LEDS; x++){
          leds[x] = CRGB(ColorFromPalette(myPal, color));
      }

      // Display the colors we just set on the actual LEDs
      FastLED.show();

      delay(20); 
  }
 
}
