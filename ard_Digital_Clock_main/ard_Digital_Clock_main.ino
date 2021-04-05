// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.

#define DIGITS 4 //Count available digits on clock
#define SEGMENTS 7 //Count segments for ONE digit
#define LEDSPERSEGMENT 4 //Count LEDs building onto ONE segment
#define NUMBERS 10 //Count available digits 0-9
#define DIGITLEDS SEGMENTS*LEDSPERSEGMENT //Count LED's per Digit

#include <Adafruit_NeoPixel.h>
#include <RTClib.h>
#include "functions_brightness.h"


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 6 // On Trinket or Gemma, suggest changing this to 1
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS DIGITS*DIGITLEDS // Popular NeoPixel ring size



//Constants
//Contains state for segments to display digits and symbols [][segments]
                      //SegmentNum: 0 1 2 3 4 5 6   Number
bool symbols [][SEGMENTS] = {{1,1,1,0,1,1,1}, //0
                             {1,0,0,0,1,0,0}, //1
                             {1,1,0,1,0,1,1}, //2
                             {1,1,0,1,1,1,0}, //3
                             {1,0,1,1,1,0,0}, //4
                             {0,1,1,1,1,1,0}, //5
                             {0,1,1,1,1,1,1}, //6
                             {1,1,0,0,1,0,0}, //7
                             {1,1,1,1,1,1,1}, //8
                             {1,1,1,1,1,1,0}, //9
                             {0,1,1,0,0,1,1}, // C
                             {1,1,1,1,0,0,0}, // °
                             {0,0,0,0,0,0,0}};//all off

//Variables
byte s, m, h, d, mon, temp, mode = 0;
byte brightness = 120;
byte symbol_show [DIGITS];   //Contains numbers to show on clock
bool pix_show[NUMPIXELS];   //Contains 0/1 (OFF/ON) for each LED/pixel on LED strip
unsigned long refreshtime;  //Variable to track passed time, to refresh display

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel points(12, 7, NEO_GRB + NEO_KHZ800);
//Setting up Real-Time-Clock to show the actual time
RTC_DS3231 rtc;

void setup() {
  Serial.begin(4800); //Initialize Serial Monitor
  pixels.begin(); // Initialize NeoPixel strip object (REQUIRED)
  points.begin(); // Initialize NeoPixel strip object (REQUIRED)
  if (! rtc.begin()) { //Tests if RTC is connected
    Serial.println("Couldn't find RTC");
    abort();
  }

  if (rtc.lostPower()) { //Tests if RTC lost power since last update. If lost new time is set to compile time.
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    abort();
  }

  //Dev-place start
  delay(100);
  
  refreshtime = millis(); 
  //Set actual time to track passed time for refresh display
}

void loop() {     
  //Read actual Real Time
  DateTime  now = rtc.now();
  mon = now.month();
  d = now.day();
  h = now.hour();
  m = now.minute();
  s = now.second();

  if(h < 8 || h > 22)mode = B10; //Change Day and Night mode
  else mode = B00;
  if(s > 25 && s < 35) mode += B1; //Change Time and Date mode

  rainbow(mode,120);
  point(mode);
  cutColors(mode);
  pixels.show();
  points.show();
}



void cutColors(byte mode){           //Sets none signal_show segments to off
  switch (mode%2) {
    case 0:
      symbol_show[3] = h / 10;
      symbol_show[2] = h - symbol_show[3] * 10;
      symbol_show[1] = m / 10;
      symbol_show[0] = m - symbol_show[1] * 10;
      if(symbol_show[3] == 0)symbol_show[3] = 12;
      break;
    case 1:
      symbol_show[3] = d / 10;
      symbol_show[2] = d - symbol_show[3] * 10;
      symbol_show[1] = mon / 10;
      symbol_show[0] = mon - symbol_show[1] * 10;
      break;
    default:
      break;
  }
  calc_pixels(symbols, symbol_show, pix_show);  //Calculates every pixel based on "pixel_show"
  //Set pixel OFF/ON based on "pix_show" and their color atm fixed to one color. Could implement array to set different color for each pixel.
  for(int i = 0; i < sizeof(pix_show); i++){
    if(!pix_show[i])pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
}



// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(byte mode, int wait) {
  if((mode >> 1)%2 == 0){
    static long firstPixelHue = 0; 
    if(firstPixelHue < 3*65536){
      for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
        // (strip.numPixels() steps):
        int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
        // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
        // optionally add saturation and value (brightness) (each 0 to 255).
        pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue, 160, brightness)));
      }
      firstPixelHue += 256;
    } else firstPixelHue = 0;
  } else if ((mode >> 1)%2 == 1){
    for (int i=0; i< pixels.numPixels(); i++){
      pixels.setPixelColor(i, pixels.Color(2,0,0));
    }
  }
  delay(wait);
}
void point(byte mode){
  for(int i=0; i<12; i++){
    if(!bitRead(mode, 1)){ //Day
      if(!bitRead(mode, 0)){
        if(s%2)points.setPixelColor(i, points.Color(10,10,10)); //Blink ON
          else points.setPixelColor(i, points.Color(5,5,5));   //Blink OFF
      } else {
        if(i>5)points.setPixelColor(i, points.Color(5,5,5));
          else points.setPixelColor(i, points.Color(0,0,0));
      }
    } else if(bitRead(mode, 1)){ //Night
      if(!bitRead(mode, 0)){
          points.setPixelColor(i, points.Color(2,0,0));   //Blink OFF
      } else {
        if(i>5)points.setPixelColor(i, points.Color(2,0,0));
          else points.setPixelColor(i, points.Color(0,0,0));
      }
    }
  }
}
