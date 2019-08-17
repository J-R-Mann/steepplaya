// Jmann
//08.11.2019
//Code uses FASTLED to drive 550 LEDs on a teapot art car.
//In total their are closer to 1000 LEDS but my arduino does not have the memory to power that many so I am running some strips in parellel

// This code incldes my package FAST LED, defines the LED data pin, specifies my LED chip, color order, and brightness
#include <FastLED.h>
#define LED_PIN 7
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B
#define NUM_LEDS 150
#define BRIGHTNESS 120

// I don't understand the difference between setting a constant and defining...
const byte ONOFF = 5;
const byte MODESELECTOR = 2;
const byte FLASH = 3 ;
const byte POT = A2 ;

int mybright = BRIGHTNESS; //save analog value

CRGB leds[NUM_LEDS];

// initializing palettes and blending
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

// initializing the effects I will step through.
int effect = 4;

// flash white when the flashy button is pressed
void alert() {
  fill_solid(leds, NUM_LEDS, CRGB::White);
  FastLED.show();
}

// This is the function that interupts with a button and runs
// alert()
void flashy_press() {
  //​
  // debouncer
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 100) {
    while (digitalRead(FLASH) == HIGH) {
      alert();
    }
  }
  last_interrupt_time = interrupt_time;
}

// This function changes a number to switch between animations
// not sure this will actually interupt a delay
void changy_press() {

  // debouncer
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 100) {
    while (digitalRead(MODESELECTOR) == HIGH) {
      effect++;
      if (effect == 5) {
        effect = 1;
      }
    }
    last_interrupt_time = interrupt_time;
  }
}
void setup() {
  // put your setup code here, to run once:
  // give the chip a sec or 3 to boot up
  delay(3000);
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER> (leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  pinMode(ONOFF, INPUT);
  pinMode(MODESELECTOR, INPUT);
  pinMode(FLASH, INPUT);
  //pinMode(POTENTIOMETER, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), changy_press, RISING);
  attachInterrupt(digitalPinToInterrupt(3), flashy_press, RISING);
}

// Here is where I can add some palettes to my code

// Gradient palette "bhw2_sunsetx_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_sunsetx.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_sunsetx_gp ) {
  0,  42, 55, 255,
  25,  73, 101, 242,
  89, 115, 162, 228,
  107, 115, 162, 228,
  114, 100, 77, 201,
  127,  86, 23, 174,
  142, 190, 32, 24,
  171, 210, 107, 42,
  255, 232, 229, 67
};
CRGBPalette16 myPal = bhw2_sunsetx_gp;

void loop() {
  // put your main code here, to run repeatedly:
  switch (effect) { 
    Serial.println(effect);
    case 1:
      backandforth();
      break;
    case 2:
      rainbows();
      break;
    case 3:
      rainbows2();
      break;
    case 4:
      redlightdistrict();
      break;
  }
}
// Mode 1 loop through a palette, back and forth 4ever
void backandforth()
{
  for ( int colorStep = 0; colorStep < 240; colorStep++ ) {
    int color = colorStep;
    // Now loop though each of the LEDs and set each one to the current color
    fill_solid(leds, NUM_LEDS, CRGB(ColorFromPalette(myPal, color)));
    // Display the colors we just set on the actual LEDs
    FastLED.show();
    if (effect > 1) {     // exit loop if state change occurs
      break;
    }
    bright();
    delay(30);
    changy_press();
  }

  for ( int colorStep = 240; colorStep < 1; colorStep-- ) {
    int color = colorStep;
    // Now loop though each of the LEDs and set each one to the current color
    fill_solid(leds, NUM_LEDS, CRGB(ColorFromPalette(myPal, color)));
    // Display the colors we just set on the actual LEDs
    FastLED.show();
    if (effect > 1) {     // exit loop if state change occurs
      break;
    }
    bright();
    delay(30);
    changy_press();
  }
}


//Fill with a rainbow state 2
void rainbows()
{
  for (int i = 0; i < NUM_LEDS; i++) {
    fill_rainbow(leds, i, 0, 5);
    addGlitter(30);
    FastLED.show();
    if (effect > 2) {     // exit loop if state change occurs
      break;}
      bright();
      delay(30);
      changy_press();
  }
}


//Fill with a rainbow state 3
void rainbows2()
{
    fill_rainbow(leds, NUM_LEDS, 0, 5);
    addGlitter(100);
    FastLED.show();
      bright();
      delay(100);
      //flashy_press();
      changy_press();
    }

// Just all red lights state 4
void redlightdistrict()
{
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));
  //   if (effect > 3) {     // exit loop if state change occurs
  //   break;
  FastLED.show();
  bright();
  //flashy_press();
  changy_press();
}


////pressing a button to incease effect
//void changeEffect() {
//  if (digitalRead (MODESELECTOR) == HIGH)
//  {
//    effect++;
//    }
//  if (effect == 5)
//  {
//    effect = 0;
//    }
//}

//glitter effect
void addGlitter( fract8 chanceOfGlitter) {
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void bright() {
  mybright = analogRead(POT); //Read and save analog value from potentiometer
  mybright = map(mybright, 0, 1023, 0, BRIGHTNESS); //Map value 0-1023 to 0-255
  FastLED.setBrightness(mybright);
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
