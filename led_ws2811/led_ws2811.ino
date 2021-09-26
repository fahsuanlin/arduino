#include <FastLED.h>

#define NUM_LEDS 9 // number of led present in your strip
#define DATA_PIN 6 // digital pin of your arduino

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
      FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);}

void loop() {
  // put your main code here, to run repeatedly:
 // Turn the LED on, then pause
  //leds[0] = CRGB::Blue;
  //leds[1] = CRGB::Red;
  //leds[2] = CRGB::Green;
  //FastLED.show();
  //delay(100);
  // Now turn the LED off, then pause
  //leds[0] = CRGB::Black;
  //FastLED.show();
  //delay(900);

   // one at a time
  for(int j = 0; j < 3; j++) {
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      memset(leds, 0, NUM_LEDS * 3);
      switch(j) {
        case 0: leds[i].r = 255; break;
        case 1: leds[i].g = 255; break;
        case 2: leds[i].b = 255; break;
      }
      FastLED.show();
      delay(1000);
    }
  }
}
