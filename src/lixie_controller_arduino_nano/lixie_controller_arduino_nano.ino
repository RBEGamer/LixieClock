// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <Wire.h>

#include "RTClib.h"

RTC_DS1307 rtc;


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        8 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 40 // Popular NeoPixel ring size



Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels



int hours = 0;
int mins = 0;
int secs = 0;


void setup() {

  Serial.begin(9600);

  
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)


  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("_rct_begin_error_");
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("_rct_isrunning_error_");
  }


  delay(100);
  DateTime now = rtc.now();
  hours = now.hour();
  mins = now.minute();
  secs = now.second();
  
}


int led_index_digits[] = {9, 0, 1, 3, 2, 4, 5, 7, 6, 8}; //0,1 2 3 4 5 6 7 8 9
int digit_offsets[] = {0, 10 ,20 ,30}; //HOUR_TENS HOUR_ONES MINUTES_TENS MINUTES_ONES



String readString;
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {
    0,
    -1
  };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}



void update_rtc() {
  rtc.adjust(DateTime(2000, 1, 1, hours, mins, 0));
  Serial.print("_t_");
  Serial.print(hours);
  Serial.print("_");
  Serial.print(mins);
  Serial.print("_");
  Serial.print(secs);
  Serial.println("_");

}








uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}



void _display(int h, int m, int col){


 
   int m_tens;
   int m_ones;
  m_tens = m / 10;      // tens now = 2
  m_ones = m % 10;      // ones now = 6 

  int h_tens;
  int h_ones;
  h_tens = h / 10;      // tens now = 2
  h_ones = h % 10;      // ones now = 6 
  
  pixels.clear(); // Set all pixel colors to 'off'

 
 
    pixels.clear(); // Set all pixel colors to 'off'

    
    pixels.setPixelColor(digit_offsets[0] + led_index_digits[h_tens], Wheel((col + col) % 255));
    pixels.setPixelColor(digit_offsets[1] + led_index_digits[h_ones], Wheel((col + col) % 255));
    pixels.setPixelColor(digit_offsets[2] + led_index_digits[m_tens], Wheel((col + col) % 255));
    pixels.setPixelColor(digit_offsets[3] + led_index_digits[m_ones], Wheel((col + col) % 255));


    
    pixels.show();   // Send the updated pixel colors to the hardware.
 
}


void loop() {



  DateTime now = rtc.now();
    hours = now.hour();
    mins = now.minute();
    secs = now.second();
 

  while (Serial.available()) {
    delay(30); //delay to allow buffer to fill
    if (Serial.available() > 0) {
      char c = Serial.read(); //gets one byte from serial buffer
      readString += c; //makes the string readString
      if (c == '\n') {
        Serial.flush();
        break;
      }
    }
  }

if (readString.length() > 0) {

  if (getValue(readString, '_', 1) == "st") {

    int tmp_hours = getValue(readString, '_', 2).toInt();
    int tmp_mins = getValue(readString, '_', 3).toInt();

    if (tmp_hours > 0 && tmp_hours < 24 && tmp_mins > 0 && tmp_mins < 60) {
      hours = tmp_hours;
      mins = tmp_mins;
      update_rtc();
    } else {
      Serial.println("_error_params.out.of.range_");
    }

  } 
  readString = "";
}





  _display(hours,mins,map(secs,0,60,0,255));
  

    delay(DELAYVAL); // Pause before next pass through loop
  
}
