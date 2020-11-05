#define DIGITS 1 //Count available digits
#define SEGMENTS 7 //Count segments for ONE digit
#define SEGMENTLEDS 6 //Count LEDs building onto ONE segment
#define NUMBERS 10 //Count available digits 0-9

#define DIGITLEDS SEGMENTS*SEGMENTLEDS //Count LED's per Digit

#include <Adafruit_NeoPixel.h>
#include "functions_brightness.h"


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS DIGITS*DIGITLEDS // Popular NeoPixel ring size

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//Constants
//Contains state for segments to display digits [digits][segments]
                     //SegmentNum: 0 1 2 3 4 5 6 
bool digits [NUMBERS][SEGMENTS] = {{1,1,1,0,1,1,1}, //0
                                  {1,0,0,0,1,0,0}, //1
                                  {1,1,0,1,0,1,1}, //2
                                  {1,1,0,1,1,1,0}, //3
                                  {1,0,1,1,1,0,0}, //4
                                  {0,1,1,1,1,1,0}, //5
                                  {0,1,1,1,1,1,1}, //6
                                  {1,1,0,0,1,0,0}, //7
                                  {1,1,1,1,1,1,1}, //8
                                  {1,1,1,1,1,1,0}};//9


//Variables
//Contains variables
int digit_show [DIGITS];
bool pix_show[NUMPIXELS];

void setup() {

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  Serial.begin(4800);
  delay(100);
  Serial.println("Started");
  
  digit_show[0] = 2;
  
  calc_pixels(digits, digit_show, pix_show);

  Serial.println("Ended");
}

void loop() {
}


/*
  // put your main code here, to run repeatedly:

  pixels.clear();  //Set  all pixel colors of 'off'
  
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }

  */
