#define DIGITS 4 //Count available digits
#define SEGMENTS 7 //Count segments for ONE digit
#define SEGMENTLEDS 6 //Count LEDs building onto ONE segment
#define NUMBERS 10 //Count available digits 0-9
#define DIGITLEDS SEGMENTS*SEGMENTLEDS //Count LED's per Digit

#include <Adafruit_NeoPixel.h>
#include <RTClib.h>
#include "functions_brightness.h"


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS DIGITS*DIGITLEDS // Popular NeoPixel ring size







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
byte digit_show [DIGITS];
bool pix_show[NUMPIXELS];
unsigned long refreshtime;
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//Setting up Real-Time-Clock to show the actual time
RTC_DS3231 rtc;

void setup() {
  Serial.begin(4800); //Initialize Serial Monitor
  pixels.begin(); // Initialize NeoPixel strip object (REQUIRED)

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  
  delay(100);
  
  digit_show[3] = 0;
  digit_show[2] = 0;
  digit_show[1] = 0;
  digit_show[0] = 0;
  
  refreshtime = millis();

}

void loop() {
  if(millis() - refreshtime > 1000){
    DateTime  now = rtc.now();

    byte h = now.hour();
    byte m = now.minute();
    digit_show[3] = h / 10;
    digit_show[2] = h - digit_show[3] * 10;
    digit_show[1] = m / 10;
    digit_show[0] = m - digit_show[1] * 10;

    /*
    // DEBUG
    Serial.print(digit_show[3]);
    Serial.print(digit_show[2]);
    Serial.print(":");
    Serial.print(digit_show[1]);
    Serial.println(digit_show[0]);
    */

    calc_pixels(digits, digit_show, pix_show);

    pixels.clear();
    
    for(int i = 0; i < sizeof(pix_show); i++){
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    }

    pixels.show();
    
    refreshtime = millis();
    //Serial.println("Done");
  }
}
