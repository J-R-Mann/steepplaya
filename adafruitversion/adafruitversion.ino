// Jmann
//08.11.2019
//Code uses FASTLED to drive 550 LEDs on a teapot art car.
//In total their are closer to 1000 LEDS but my arduino does not have the memory to power that many so I am running some strips in parellel

// This code incldes my package FAST LED, defines the LED data pin, specifies my LED chip, color order, and brightness
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN 7
#define BRIGHTNESS 180
#define LED_COUNT 150

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//set constants
const byte ONOFF = 5;
const byte MODESELECTOR = 2;
const byte FLASH = 3 ;
const byte POT = A2 ;
int mybright = BRIGHTNESS; //save analog value
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

// set volatiles for my button
volatile long debounce_time = 0;
volatile long current_time = 0;
volatile int effect = 1;

//clock for functions. Maybe I can use current_time?
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

//delays for each function. number refers to state.
long delayTime1 = 40;
long delayTime2 = 50;
long delayTime3 = 20;

// This is the function that interupts with a button and runs
// alert()
void flashy_press() {
    while (digitalRead(FLASH) == HIGH) 
   {
    alert();
   }
  }
  
// This function changes a number to switch between animations
void changy_press() {
  // debouncer
  current_time = millis();
  if (current_time - debounce_time > 200) {
      effect++;
      if (effect == 6) {
        effect = 1;
      }
  }
    debounce_time = current_time;
}

// setup function runs once
void setup() {
  // put your setup code here, to run once:
  // give the chip a sec or 3 to boot up
  delay(2000);
  Serial.begin(9600);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS); // Set BRIGHTNESS to about 1/5 (max = 255)
  pinMode(ONOFF, INPUT);
  pinMode(MODESELECTOR, INPUT);
  pinMode(FLASH, INPUT);
  attachInterrupt(digitalPinToInterrupt(MODESELECTOR), changy_press, RISING );
  attachInterrupt(digitalPinToInterrupt(FLASH), flashy_press, RISING);
}
void loop() {
  // put your main code here, to run repeatedly:
  switch (effect) {
    case 1:
      theaterChaseRainbow(50);
      break;
    case 2:
      rainbows(20);
      break;
    case 3:
      rainbows2(50);
      break;
    case 4:
      redlightdistrict();
      break;
    case 5:
      soundcheck ();
      break;
  }
}
// Mode 1 loop through a palette, back and forth 4ever
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      bright();
      strip.show();                // Update strip with new contents
      if (effect != 1) {     // exit loop if state change occurs
      break;
      }
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
//

void rainbows(int wait)
{
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    bright();
    strip.show(); // Update strip with new contents
    if (effect != 2) {     // exit loop if state change occurs
    break;
    }
    delay(wait);  // Pause for a moment
  }
}

void rainbows2(int wait)
{
  bright();
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    addGlitter(50);
    bright();
    if (effect != 3) {     // exit loop if state change occurs
    break;
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
// Just all red lights state 4
void redlightdistrict()
{
  bright();
  strip.fill(strip.Color(255, 0, 0));
  strip.show();
}

void soundcheck ()
{
  while(effect = 5)
  {
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level

   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;

   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
   volts = volts *100;
   mybright = map(volts, 0, 250, 20, BRIGHTNESS);
   //Serial.println(volts);
   Serial.println(mybright);
   strip.setBrightness(mybright);
   if (effect != 5) {     // exit loop if state change occurs
   break;
   }
   if(mybright > 140)
   {
   strip.fill(strip.Color(255, 255, 255));
   strip.show();
   }
   else if (mybright > 100) {
   strip.fill(strip.Color(255, 0, 0));
   strip.show();
   }
   else if(mybright > 75){
   strip.fill(strip.Color(125, 0, 255));
   strip.show();
   }
   else {
   strip.fill(strip.Color(255, 0, 255));
   strip.show();
   }
}
}

//glitter effect with how many glitters are we talking here
void addGlitter(int Count) {
    for (int i = 0; i < Count; i++)
    {
    strip.setPixelColor(random(LED_COUNT),255,255,255);
    }
  }

// Potentiometer to set brightness
void bright() {
  mybright = analogRead(POT); //Read and save analog value from potentiometer
  mybright = map(mybright, 0, 1023, 10, BRIGHTNESS); //Map value 0-1023 to 10-brightness
  strip.setBrightness(mybright);
}

// flash white when the flashy button is pressed
void alert() {
  strip.fill(strip.Color(255, 255, 255), 0);
  strip.show();
}


//// old examples
//// intialize the second while loop to descend colors
//  currentMillis = millis();
//  previousMillis = currentMillis;
//  colorStep = 240;
//  color = 240 ;
//  while (colorStep > 1) 
//  { 
//    if (effect != 1) {     // exit loop if state change occurs
//    break;
//    }
//    bright(); 
//  currentMillis = millis();  
//  if (currentMillis - previousMillis >= delayTime1)
//  {
//    colorStep--;
//    Serial.println(colorStep);
//    int color = colorStep;
//    previousMillis = currentMillis;
//    
//    }
//  else {    
//    fill_solid(leds, NUM_LEDS, CRGB(ColorFromPalette(myPal, color)));
//    FastLED.show();
//  }
//  }
//}
////Fill with a rainbow state 2
//void rainbows()
//{
//  currentMillis = millis();
//  previousMillis = currentMillis;
//  i=0;
//  while ( i < NUM_LEDS) {
//    if (effect != 2) {  // exit loop if state change occurs
//      break;
//    }
//    bright();
//    currentMillis = millis();
//    if (currentMillis - previousMillis >= delayTime2)
//    {
//      i++;
//      previousMillis = currentMillis;
//    }
//    else {
//      fill_rainbow(leds, i, 0, 5);
//      addGlitter(80);
//      FastLED.show();
//    }
//  }
//  FastLED.clear();
//}
//
//
////Fill with a rainbow state 3
//void rainbows2()
//{
//  unsigned long currentMillis = millis();
//  bright();
//  if (currentMillis - previousMillis >= delayTime3)
//  { 
//    fill_rainbow(leds, NUM_LEDS, 0, 10);
//    addGlitter(80);
//    previousMillis = currentMillis;
//  }
//  else {
//    FastLED.show();
//  }
//}
