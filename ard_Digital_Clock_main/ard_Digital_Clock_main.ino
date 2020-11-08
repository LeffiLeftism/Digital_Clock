#define DIGITS 4 //Count available digits on clock
#define SEGMENTS 7 //Count segments for ONE digit
#define LEDSPERSEGMENT 6 //Count LEDs building onto ONE segment
#define NUMBERS 10 //Count available digits 0-9
#define DIGITLEDS SEGMENTS*LEDSPERSEGMENT //Count LED's per Digit

#include <Adafruit_NeoPixel.h>
#include <RTClib.h>
#include "functions_brightness.h"


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1
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
                             {1,1,1,1,0,0,0}};// °

//Variables
//Contains variables 
byte symbol_show [DIGITS];   //Contains numbers to show on clock
bool pix_show[NUMPIXELS];   //Contains 0/1 (OFF/ON) for each LED/pixel on LED strip
unsigned long refreshtime;  //Variable to track passed time, to refresh display

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
  
  symbol_show[3] = 2;
  symbol_show[2] = 6;
  symbol_show[1] = 10;
  symbol_show[0] = 11;

  //Def-place end

  //Add function for startup Visualisation
  
  refreshtime = millis(); //Sets actual time to track passed time for refresh display

}

void loop() {

  //Add change between time and temperature
  
  if(millis() - refreshtime > 1000){    //Check if refreshtime is over
    DateTime  now = rtc.now();     //Read actual Real Time

  //Set "symbol_show" to show actual Real Time
    byte h = now.hour();
    byte m = now.minute();
    symbol_show[3] = h / 10;
    symbol_show[2] = h - symbol_show[3] * 10;
    symbol_show[1] = m / 10;
    symbol_show[0] = m - symbol_show[1] * 10;

    if(false){
      // DEBUG
      Serial.print(symbol_show[3]);
      Serial.print(symbol_show[2]);
      Serial.print(":");
      Serial.print(symbol_show[1]);
      Serial.println(symbol_show[0]);
    }
    


    calc_pixels(symbols, symbol_show, pix_show);  //Calculates every pixel based on "pixel_show"

    pixels.clear(); //Clears all pixels (not sure if needed, if I overwrite every pixel with new informations)

    //Set pixel OFF/ON based on "pix_show" and their color atm fixed to one color. Could implement array to set different color for each pixel.
    for(int i = 0; i < sizeof(pix_show); i++){
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    }

    pixels.show();    //Show the pixel brightness and color set
    
    refreshtime = millis(); //reset refreshtime to wait again
    //Serial.println("Done");

    if(false){
      //Testplace start
      Serial.print("Temperature: ");
      Serial.print(rtc.getTemperature());
      Serial.println(" C°");
      //Testplace end
    }
  }
}
