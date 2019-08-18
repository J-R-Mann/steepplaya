// Jmann
//08.11.2019
//Code uses FASTLED to drive 550 LEDs on a teapot art car.
//In total their are closer to 1000 LEDS but my arduino does not have the memory to power that many so I am running some strips in parellel

// This code incldes my package FAST LED, defines the LED data pin, specifies my LED chip, color order, and brightness
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#define LED_PIN 7
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B
#define BRIGHTNESS 120
#define NUM_LEDS 150

// I don't understand the difference between setting a constant and defining...
const byte ONOFF = 5;
const byte MODESELECTOR = 2;
const byte FLASH = 3 ;
const byte POT = A2 ;
int mybright = BRIGHTNESS; //save analog value

// for my button
volatile long debounce_time = 0;
volatile long current_time = 0;

//clock for functions. Maybe I can use current_time?
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

//delays for each function. number refers to state.
long delayTime1 = 40;
long delayTime2 = 50;
long delayTime3 = 20;

//initializing for my first function
int colorStep = 0;
int color = 0 ;
int i=0;

CRGB leds[NUM_LEDS];

// initializing palettes and blending
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

// initializing the effects I will step through.
volatile int effect = 1;

// This is the function that interupts with a button and runs
// alert()
void flashy_press() {
  // debouncer
  current_time = millis();
  if (current_time - debounce_time > 20) {
    while (digitalRead(FLASH) == HIGH) {
    Serial.println("x");
    alert();
   }
  }
    debounce_time = current_time;
}

// This function changes a number to switch between animations
// not sure this will actually interupt a delay
void changy_press() {
  // debouncer
  current_time = millis();
  if (current_time - debounce_time > 200) {
      effect++;
      if (effect == 5) {
        effect = 1;
      }
  }
    debounce_time = current_time;
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
  attachInterrupt(digitalPinToInterrupt(MODESELECTOR), changy_press, RISING );
  attachInterrupt(digitalPinToInterrupt(FLASH), flashy_press, CHANGE);
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
// intialize the first while loop to ascend colors
  currentMillis = millis();
  previousMillis = currentMillis;
  colorStep = 0;
  color = 0 ;
  while (colorStep < 240) 
  {
    if (effect != 1) {     // exit loop if state change occurs
    break;
    }
    bright(); 
  currentMillis = millis();
  if (currentMillis - previousMillis >= delayTime1)
  {
    colorStep++;
    Serial.println(colorStep);
    color = colorStep;
    previousMillis = currentMillis;
    }
  else {    
    fill_solid(leds, NUM_LEDS, CRGB(ColorFromPalette(myPal, color)));
    FastLED.show();
  }
  }

// intialize the second while loop to descend colors
  currentMillis = millis();
  previousMillis = currentMillis;
  colorStep = 240;
  color = 240 ;
  while (colorStep > 1) 
  { 
    if (effect != 1) {     // exit loop if state change occurs
    break;
    }
    bright(); 
  currentMillis = millis();  
  if (currentMillis - previousMillis >= delayTime1)
  {
    colorStep--;
    Serial.println(colorStep);
    int color = colorStep;
    previousMillis = currentMillis;
    
    }
  else {    
    fill_solid(leds, NUM_LEDS, CRGB(ColorFromPalette(myPal, color)));
    FastLED.show();
  }
  }
}
//Fill with a rainbow state 2
void rainbows()
{
  currentMillis = millis();
  previousMillis = currentMillis;
  i=0;
  while ( i < NUM_LEDS) {
    if (effect != 2) {  // exit loop if state change occurs
      break;
    }
    bright();
    currentMillis = millis();
    if (currentMillis - previousMillis >= delayTime2)
    {
      i++;
      previousMillis = currentMillis;
    }
    else {
      fill_rainbow(leds, i, 0, 5);
      addGlitter(80);
      FastLED.show();
    }
  }
  FastLED.clear();
}


//Fill with a rainbow state 3
void rainbows2()
{
  unsigned long currentMillis = millis();
  bright();
  if (currentMillis - previousMillis >= delayTime3)
  { 
    fill_rainbow(leds, NUM_LEDS, 0, 10);
    addGlitter(80);
    previousMillis = currentMillis;
  }
  else {
    FastLED.show();
  }
}

// Just all red lights state 4
void redlightdistrict()
{
  bright();
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));
  FastLED.show();
  //changy_press();
}

//glitter effect
void addGlitter( fract8 chanceOfGlitter) {
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void bright() {
  mybright = analogRead(POT); //Read and save analog value from potentiometer
  mybright = map(mybright, 0, 1023, 10, BRIGHTNESS); //Map value 0-1023 to 10-brightness
  FastLED.setBrightness(mybright);
}


// flash white when the flashy button is pressed
void alert() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 255, 255));
  FastLED.show();
}

///////////////SOME EXAMPLES ////////////////////////////////////////////////////  
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
