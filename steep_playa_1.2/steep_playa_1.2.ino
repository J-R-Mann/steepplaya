// Jmann
//08.11.2019
//Code uses FASTLED to drive 550 LEDs on a teapot art car.
//In total their are closer to 1000 LEDS but my arduino does not have the memory to power that many so I am running some strips in parellel

// This code incldes my package FAST LED, defines the LED data pin, specifies my LED chip, color order, and brightness
#include <FastLED.h>
#define LED_PIN 7
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B
#define NUM_LEDS 549
#define BRIGHTNESS 120
#define ONOFF 5
#define MODESELECTOR 2
#define FLASH 3
#define POTENTIOMETER 4

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
int effect = 1;

void ICACHE_RAM_ATTR flashy_press() {
//​
 // debouncer
static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 100) {
    while(digitalRead(FLASH) == LOW) {
      alert();
    }
  }
  last_interrupt_time = interrupt_time; 
}

void ICACHE_RAM_ATTR changy_press() {
//​
 // debouncer
static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 100) {
    while(digitalRead(MODESELECTOR) == LOW) {
    effect++;
    if (effect == 5) {
      effect = 1;
    }
  }
  last_interrupt_time = interrupt_time; 
}

void setup() {
  // put your setup code here, to run once:
  // give the chip a sec or 3 to boot up
    delay(3000);
    Serial.begin(57600);
    LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER> (leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);   
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    pinMode(ONOFF, INPUT);
    //pinMode(MODESELECTOR, INPUT);
    //pinMode(FLASH, INPUT);
    pinMode(POTENTIOMETER, INPUT);
    attachInterrupt(digitalPinToInterrupt(MODESELECTOR), flashy_press, CHANGE);
    attachInterrupt(digitalPinToInterrupt(FLASH), changy_press, CHANGE);
}

// Here is where I can add some palettes to my code

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
switch(effect){
 case 1:
  backandforth();
 case 2:
  rainbows();
 case 3:
  redlightdistrict();
}

// Mode 1 loop through a palette, back and forth 4ever
void backandforth()
{
  for( int colorStep=0; colorStep<240; colorStep++ ) {
      int color = colorStep;
      // Now loop though each of the LEDs and set each one to the current color
      fill_solid(leds, NUM_LEDS,CRGB(ColorFromPalette(myPal, color)));
      // Display the colors we just set on the actual LEDs
      FastLED.show();
      delay(70); 
  }

    for( int colorStep=240; colorStep<1; colorStep-- ) {
      int color = colorStep;
      // Now loop though each of the LEDs and set each one to the current color
      fill_solid(leds, NUM_LEDS,CRGB(ColorFromPalette(myPal, color)));
      // Display the colors we just set on the actual LEDs
      FastLED.show();
      delay(70); 
  }
}

//Fill with a rainbow
void rainbows()
{
 for(int i=0;i<NUM_LEDS;i++){
   fill_rainbow(leds,i, 0, 5);
   addGlitter(80);
   FastLED.show();
   delay(20);
 }
}

void redlightdistrict()
{
fill_solid(leds, NUM_LEDS, CRGB(255,0,0)); 
FastLED.show();
}


//pressing a button to incease effect
void changeEffect() {
  if (digitalRead (MODESELECTOR) == HIGH) 
  {
    effect++;
    }
  if (effect == 5) 
  {
    effect = 0;
    }
}

//glitter effect
void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;}
}



void alert() {
fill_solid(leds, NUM_LEDS, CRGB::White);
FastLED.show();
}

// Example of lighting individual LEDS
//  leds[0] = CRGB::HotPink;
//  leds[2] = CRGB::HotPink;
//  FastLED.show();
//  delay(10);

// Example of lighting all LEDS
//  fill_solid(leds, NUM_LEDS, 0xFF61A5);
//  FastLED.show();
//  delay(1000);
//  FastLED.clear();
//FastLED.clear();

// Example of chasinf LEDs
// for(int i=0;i<NUM_LEDS;i++){  
//   leds[i-2] = 0xFF61A5;
//   leds[i-1] = 0x3CFF57;
//   leds[i] = 0xFFDA54;
//   FastLED.show();
//   delay(100);
//   FastLED.clear();
// }

//FastLED.clear();  
// Fill with palette
// for(int i=0;i<NUM_LEDS;i++){
//   fill_palette(leds,i, 0, 5,currentPalette,255,currentBlending);
//   FastLED.show();
//   delay(20);
// }
