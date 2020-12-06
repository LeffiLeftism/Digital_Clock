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
byte modus = 0;
byte s, m, h, d, mon, temp;
byte brightness = 0;
byte symbol_show [DIGITS];   //Contains numbers to show on clock
bool pix_show[NUMPIXELS];   //Contains 0/1 (OFF/ON) for each LED/pixel on LED strip
unsigned long refreshtime;  //Variable to track passed time, to refresh display

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
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
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) { //Tests if RTC lost power since last update. If lost new time is set to compile time.
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  //Dev-place start
  delay(100);
  

  //Def-place end

  //Add function for startup Visualisation
  
  refreshtime = millis(); //Sets actual time to track passed time for refresh display

  DateTime  now = rtc.now();     //Read actual Real Time
  h = now.hour();
  m = now.minute();
}

void loop() {

  if(h > 22 || h < 7){
    for(int i = 0; i < NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(1,0,0));
      if(i<12){
        if(s%2){
          points.setPixelColor(i, points.Color(1,0,0));
        } else {
          points.setPixelColor(i, points.Color(10,0,0));
        }
      }
    }
    cutColors();
    pixels.show();
    points.show();
  } else {  
    //Serial.println("Rainbow");
    rainbow(120);
  }  
  /*
  Serial.println("theaterChase");
  theaterChase(pixels.Color(  0,   0, 127), 49);
  
  Serial.println("theaterChaseRainbow");
  theaterChaseRainbow(35);
  */
}


//Sets none signal_show segments to off
void cutColors(){
  DateTime  now = rtc.now();     //Read actual Real Time
  //Changing displayed inforamtion (Time, Date, Temperature)
  s = now.second();
  //Changing which information is displayed by Real Time second
  if(s <=15 || s >45) modus = 0;
  else if(s > 15 && s <=30) modus = 1;
  else if(s > 30 && s < 45) modus = 2;
  if(h > 22 || h < 6){brightness = 24;} else {brightness = 255;}
    refreshtime = millis();
  //Updating informations for display
  switch (modus) {
    case 0:
      h = now.hour();
      m = now.minute();
      symbol_show[3] = h / 10;
      symbol_show[2] = h - symbol_show[3] * 10;
      symbol_show[1] = m / 10;
      symbol_show[0] = m - symbol_show[1] * 10;
      if(symbol_show[3] == 0){
        symbol_show[3] = 12;
      }
      break;
    case 1:
      d = now.day();
      mon = now.month();
      symbol_show[3] = d / 10;
      symbol_show[2] = d - symbol_show[3] * 10;
      symbol_show[1] = mon / 10;
      symbol_show[0] = mon - symbol_show[1] * 10;
      break;
    case 2:
      temp = rtc.getTemperature();
      symbol_show[3] = temp / 10;
      symbol_show[2] = temp - symbol_show[3] * 10;
      symbol_show[1] = 11;
      symbol_show[0] = 10;
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



/*
functions to test:
colorWipe(pixels.Color(  0, 255,   0), 50);    // Green
theaterChase(strip.Color(  0,   0, 127), 50); // Blue
rainbow(10);
theaterChaseRainbow(50);
*/
// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<pixels.numPixels(); c += 3) {
        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      cutColors();
      pixels.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue, 255, brightness)));
      if(i<12){
        if(s%2 && modus == 0){
          points.setPixelColor(i, points.Color(60,60,60));
        } else if(modus == 0){
          points.setPixelColor(i, points.Color(10,10,10));
        } else if(modus == 1){
          if(i>5){
            points.setPixelColor(i, points.Color(10,10,10));
          } else {
            points.setPixelColor(i, points.Color(0,0,0));
          }
        } else if(modus == 2){
          points.setPixelColor(i, points.Color(0,0,0));
        }
      }
    }
    cutColors();
    pixels.show(); // Update strip with new contents
    points.show();
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<pixels.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / pixels.numPixels();
        uint32_t color = pixels.gamma32(pixels.ColorHSV(hue)); // hue -> RGB
        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      cutColors();
      pixels.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
