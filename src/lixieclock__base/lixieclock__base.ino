//FH AACHEN LIXIECLOCK https://github.com/RBEGamer/LixieClock/
//--- CHANGELOG ----
//VERSION 1.0 Marcel Ochsendorf info@marcelochsendorf.com 27.01.2022


#define VERSION "1.0"
#include <Arduino.h>

#define PCB_V1_FIX //DEFINE THIS TO AVOID PIXEL ERRORS ON THE PCB V1; ON THIS PCB VERISION THE SEGMENT PAIRS 7,8 and 3,4 ARE SWAPPED

//LED PIN CONFIG
#ifdef ESP8266
#define NEOPIXEL_PIN D8
#endif

#ifdef ESP32
#define NEOPIXEL_PIN 4
#endif





//ON THE ESP32 PCB VERSION WITH ESP32 IS AN ADDITIONAL FIRST LED ON THE BOTTOM SIDE OF THE PCB FOR STATUS INDICATION
//SO FOR INDEXING THE CLOCK DIGITS AN OFFSET OF 1 MUST BE ADDED
//IF YOU A RE USING A D1 MINI DIRECTLY CONNECTED TO THE FIST DIGIT LED THE OFFSET IS ZERO
#if defined(ESP32)
#define DEFAULT_LED_OFFSET 1
const int led_offset = 1;
#elif defined(ESP8266)
const int led_offset = 0; //WEMOS D1 MINI VERSION
#else
const int led_offset = 0;
#endif




#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif



// NEOPIXEL CONF ------------------------------

const int COUNT_CLOCK_DIGITS = 6; // 2 4 OR 6 DIGITS ARE SUPPORTED
const int NEOPIXEL_DIGIT_OFFSET = 10;
const int NUM_NEOPIXELS = (COUNT_CLOCK_DIGITS*10) + NEOPIXEL_DIGIT_OFFSET; //10 leds per digit

#if defined(PCB_V1_FIX)
const int led_index_digits[] = {0, 9, 8, 7, 6, 5, 4, 3, 2, 1}; //PIXEL INDEX OFFSET FOR EACH DIGIT STARTING AT 0,1 2 3 4 5 6 7 8 9
#else
const int led_index_digits[] = {0, 9, 8, 6, 7, 5, 4, 2, 3, 1}; //PIXEL INDEX OFFSET FOR EACH DIGIT STARTING AT 0,1 2 3 4 5 6 7 8 9
#endif
const int digit_offsets[6] = {0, 10 ,20 ,30, 40, 50}; //HOUR_TENS HOUR_ONES MINUTES_TENS MINUTES_ONES


Adafruit_NeoPixel pixels(NUM_NEOPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);


uint32_t Wheel(int WheelPos, int _bright) {
  
   if(_bright > 255){
      _bright = 255;
   }else if(_bright < 0){
    _bright = 0;
    }
    
   const float brgth_scale = _bright / 255.0;
    
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color((255 - WheelPos * 3) * brgth_scale, 0, (WheelPos * 3) * brgth_scale);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, (WheelPos * 3) * brgth_scale, (255 - WheelPos * 3) * brgth_scale);
  }
  WheelPos -= 170;
  return pixels.Color((WheelPos * 3) * brgth_scale, (255 - WheelPos * 3) * brgth_scale, 0);
}

uint32_t digit_color(int _val,int _index, bool _banked, int _base_color, int _bright){
  //EACH DIGIT PAIR SHOULS HAVE AN DIFFERENT COLOR SO 
    const int MAX_COLOR = 255;
    const int color_offset_per_digit = MAX_COLOR / COUNT_CLOCK_DIGITS*2;
   
   if(_banked && _val <= 0){
      return pixels.Color(0, 0, 0);
   }else{
      return Wheel((_base_color + color_offset_per_digit*_index) % MAX_COLOR, _bright);
   }
}

//h: 0-99, m:0-99, s:0-99 col: 0-255, _bright: 0-255, _disable_leading_zero:0-1
//_disable_leading_zero =>
void update_clock_display(int h, int m, int s, int col, int _bright, bool _disable_leading_zero){

  
    //UPDATE NEOPIXEL
    //SPLIT HOURS MINS,.. INTO SEPERATE DIGITS
    const int m_tens = m / 10;      // tens now = 2
    const int m_ones = m % 10;      // ones now = 6 

    const int h_tens = h / 10;      // tens now = 2
    const int h_ones = h % 10;      // ones now = 6 
    
    const int s_tens = s / 10;
    const int s_ones = s % 10;
  
    pixels.clear();
    
    //JUST INDICATE OVER THE PCB LED THAT THE CLOCK IS WOKRING
    if(led_offset > 0){
      pixels.setPixelColor(0,digit_color(0,0,false, col, _bright));
    }
    
    if(COUNT_CLOCK_DIGITS >= 2){
      pixels.setPixelColor(digit_offsets[0] + led_index_digits[h_tens] + led_offset, digit_color(h_tens,0,_disable_leading_zero, col, _bright));
      pixels.setPixelColor(digit_offsets[1] + led_index_digits[h_ones] + led_offset, digit_color(h_ones,0,_disable_leading_zero, col, _bright));
    }
 
    if(COUNT_CLOCK_DIGITS >= 4){
      pixels.setPixelColor(digit_offsets[2] + led_index_digits[m_tens] + led_offset, digit_color(m_tens,1,_disable_leading_zero, col, _bright));
      pixels.setPixelColor(digit_offsets[3] + led_index_digits[m_ones] + led_offset,digit_color(m_ones,1,_disable_leading_zero, col, _bright));
    }
 
    if(COUNT_CLOCK_DIGITS >= 6){
      pixels.setPixelColor(digit_offsets[4] + led_index_digits[s_tens] + led_offset, digit_color(s_tens,2,_disable_leading_zero, col, _bright));
      pixels.setPixelColor(digit_offsets[5] + led_index_digits[s_ones] + led_offset, digit_color(s_ones,2,_disable_leading_zero, col, _bright));
    }
    
    pixels.show();   // Send the updated pixel colors to the hardware.
 
}


void test_digits(){
  pixels.clear();

    for(int j = 0; j < 5; j++){
    for(int i = 0; i < NUM_NEOPIXELS; i++){
      pixels.clear();
      pixels.setPixelColor(i, Wheel(j*40, 255));
      pixels.show();
      delay(50);
    }
    }

    pixels.clear();
  }
  
void setup() {
    Serial.begin(9600);
   //SETUP NEOPIXELS
    pixels.begin();
    pixels.clear();
    test_digits();
}


int c = 0;

void loop() {

    update_clock_display(c%99, c%99, c%99, map(c%99,0,99,0,255), 255, false);

    c++;

    delay(1000);
}
