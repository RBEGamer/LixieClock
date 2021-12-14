
#define VERSION "1.0"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <FS.h> //Include File System Headers
#include <TimeLib.h>
#include <NTPClient.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#define ENABLE_I2C_RTC

#ifdef ENABLE_I2C_RTC
#include <Wire.h>
#include "RTClib.h"
#endif

String time_last = "not synced";
// CONFIG -------------------------------------
#define WEBSERVER_PORT 80 // set the port for the webserver eg 80 8080
#define MDNS_NAME "lixieclock" // set hostname
#define WEBSITE_TITLE "Lixie Clock Configuration" // name your device
#define SERIAL_BAUD_RATE 9600
#define NTP_SEND_TIME_INTERVAL 5 //sende zeit an uhr all x minuten

#define DEFAULT_NTP_SERVER "pool.ntp.org"
#define DEFAULT_TIMEZONE 1
#define DEFAULT_MQTT_BROKER "192.168.178.89"
#define DEFAULT_MQTT_TOPIC "/iot/9770941/humidity"
#define DEFAULT_MQTT_BROKER_PORT "1883"

#define NEOPIXEL_PIN 8
#define NUMPIXELS 60
// END CONFIG ---------------------------------



// NEOPIXEL CONF ------------------------------
const int COUNT_CLOCK_DIGITS = 6; // 2 4 OR 6 DIGITS ARE SUPPORTED
const int led_index_digits[] = {9, 0, 1, 3, 2, 4, 5, 7, 6, 8}; //PIXEL INDEX OFFSET FOR EACH DIGIT STARTING AT 0,1 2 3 4 5 6 7 8 9
const int digit_offsets[6] = {0, 10 ,20 ,30, 40, 50}; //HOUR_TENS HOUR_ONES MINUTES_TENS MINUTES_ONES

// END NEOPIXEL CONF ---------------------------
//FILES FOR STORiNG CONFIGURATION DATA
const char* file_ntp_server = "/file.txt";
const char* file_timezone = "/timezone.txt";
const char* file_syncmode = "/syncmode.txt";
const char* file_mqtt_server = "/mqttbroker.txt";
const char* file_mqtt_topic = "/mqtttopic.txt";
const char* file_mqtt_broker_port = "/mqttbrokerport.txt";





//VARS
int sync_mode = 0;
int timezone = 0;
String ntp_server_url = "";
String mqtt_broker_url = "";
String mqtt_topic = "";
String mqtt_broker_port = "1883";


long long last = 0;
long long last_abi = 0;

int rtc_hours = 0;
int rtc_mins = 0;
int rtc_secs = 0;

int mqtt_hours = 0;
int mqtt_mins = 0;
int mqtt_secs = 0;

bool abi_started = false;
int abi_counter = 0;
const int ABI_COUNTER_MAX = COUNT_CLOCK_DIGITS * 6;




// INSTANCES --------------------------------------------
ESP8266WebServer server(WEBSERVER_PORT);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,DEFAULT_NTP_SERVER,0,60000);
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#ifdef ENABLE_I2C_RTC
RTC_DS1307 rtc;
#endif






const String phead_1 = "<!DOCTYPE html><html><head><title>";
const String phead_2 = "</title>"
                       "<meta http-equiv='content-type' content='text/html; charset=utf-8'>"
                       "<meta charset='utf-8'>"
                       "<link "
                       "href='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.16/themes/base/"
                       "jquery-ui.css' rel=stylesheet />"
                       "<script "
                       "src='http://ajax.googleapis.com/ajax/libs/jquery/1.6.4/jquery.min.js'></"
                       "script>"
                       "<script "
                       "src='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.16/"
                       "jquery-ui.min.js'></script>"
                       "<style>"
                       "html, body {"
                       "  background: #F2F2F2;"
                       " width: 100%;"
                       " height: 100%;"
                       " margin: 0px;"
                       " padding: 0px;"
                       " font-family: 'Verdana';"
                       " font-size: 16px;"
                       " color: #404040;"
                       " }"
                       "img {"
                       " border: 0px;"
                       "}"
                       "span.title {"
                       " display: block;"
                       " color: #000000;"
                       " font-size: 30px;"
                       "}"
                       "span.subtitle {"
                       " display: block;"
                       " color: #000000;"
                       " font-size: 20px;"
                       "}"
                       ".sidebar {"
                       " background: #FFFFFF;"
                       " width: 250px;"
                       " min-height: 100%;"
                       " height: 100%;"
                       " height: auto;"
                       " position: fixed;"
                       " top: 0px;"
                       " left: 0px;"
                       " border-right: 1px solid #D8D8D8;"
                       "}"
                       ".logo {"
                       " padding: 25px;"
                       " text-align: center;"
                       " border-bottom: 1px solid #D8D8D8;"
                       "}"
                       ".menu {"
                       " padding: 25px 0px 25px 0px;"
                       " border-bottom: 1px solid #D8D8D8;"
                       "}"
                       ".menu a {"
                       " padding: 15px 25px 15px 25px;"
                       " display: block;"
                       " color: #000000;"
                       " text-decoration: none;"
                       " transition: all 0.25s;"
                       "}"
                       ".menu a:hover {"
                       " background: #0088CC;"
                       " color: #FFFFFF;"
                       "}"
                       ".right {"
                       " margin-left: 250px;"
                       " padding: 50px;"
                       "}"
                       ".content {"
                       " background: #FFFFFF;"
                       " padding: 25px;"
                       " border-radius: 5px;"
                       " border: 1px solid #D8D8D8;"
                       "}"
                       "</style>";

const String pstart = "</head>"
                      "<body style='font-size:62.5%;'>"
                      "<div class='sidebar'>"
                      "<div class='logo'>"
                      "<span class='title'>LixieCLock</span>"
                      "<span class='subtitle'>- Configuration -</span>"
                      "</div>"
                      "<div class='menu'>"
                      "<a href='index.html'>Settings</a>"
                      "</div>"
                      "</div>"
                      "<div class='right'>"
                      "<div class='content'>";

const String pend = "</div>"
                    "</div>"
                    "</body>"
                    "</html>";

String last_error = "";

// ONLY READ THE FIRST LINE UNTIL NEW LINE !!!!!
String read_file(const char* _file, String _default = "")
{
    File f = SPIFFS.open(_file, "r");
    String tmp = _default;
    if (!f) {
        last_error = "open filesystem file_ntp_server failed";
    }
    else {
        tmp = f.readStringUntil('\n');
        last_error = "read from FS:" + String(_file) + " " + tmp;
    }
    return tmp;
}

void restore_eeprom_values()
{
    ntp_server_url = read_file(file_ntp_server,DEFAULT_NTP_SERVER);
    timezone = read_file(file_timezone, String(DEFAULT_TIMEZONE)).toInt();
    sync_mode = read_file(file_syncmode, "1").toInt();
    mqtt_broker_url = read_file(file_mqtt_server,DEFAULT_MQTT_BROKER);
    mqtt_topic = read_file(file_mqtt_topic,DEFAULT_MQTT_TOPIC);
    mqtt_broker_port = read_file(file_mqtt_broker_port, String(DEFAULT_MQTT_BROKER_PORT));
}

bool write_file(const char* _file, String _content)
{
    File f = SPIFFS.open(_file, "w");
    if (!f) {
        last_error = "Oeffnen der Datei fehlgeschlagen";
        return -1;
    }
    f.print(_content);
    f.close();
    return 0;
}

void save_values_to_eeprom()
{
    write_file(file_ntp_server, ntp_server_url);
    write_file(file_syncmode, String(sync_mode));
    write_file(file_timezone, String(timezone));
    write_file(file_mqtt_server, mqtt_broker_url);
    write_file(file_mqtt_topic, mqtt_topic);
    write_file(file_mqtt_broker_port, mqtt_broker_port);
}


uint32_t Wheel(int WheelPos, int _bright) {
  
   if(_bright > 255){
      _bright = 255;
    }
 
    if(_bright < 0){
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


void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  String tmp = "";
  if(length > 5){
    length = 5;
  }
  for (int i = 0; i < length; i++) {
    tmp += (char)payload[i];
  }
  last_error = "GOT MQTT MSG: " + tmp;

  //IF MQTT DISPLAY IS ENABLES SEND IT TO CLOCK
  if(sync_mode == 2 && tmp != ""){
    float ff = tmp.toFloat();
    if (!isnan(ff)) {
       parse_mqtt_float_to_digits(ff);
    }
  }
}


void mqtt_reconnect() {
  // Loop until we're reconnected
 if(client.connected() || sync_mode != 2) {return;}
    // Attempt to connect
    if (client.connect((MDNS_NAME + String(ESP.getChipId())).c_str())) {
      last_error = "MQTT CLICNET CONNCTED";
      client.subscribe(mqtt_topic.c_str());
    } else {
      last_error = "MQTT CLCIENT CONECT FAILED WITH" + String(client.state());
  }
}



void parse_mqtt_float_to_digits(float _f){
    int whole = (int)_f;
    int remainder = (_f - whole) * 1000;

    if(whole < 0){
      whole = 0;
    }

    const int ones = (whole%10);
    const int tens = ((whole/10)%10);
    const int hundreds = ((whole/100)%10);
    const int thousands = (whole/1000);
    
    //SAVE AS SPLITTED REFORMATTED HOUR MINS VARIABLES
    mqtt_hours = thousands*10 + hundreds;
    mqtt_mins = tens*10 + ones;
}



void update_rtc() {
  #ifdef ENABLE_I2C_RTC
  rtc.adjust(DateTime(2000, 1, 1, rtc_hours, rtc_mins, rtc_secs));
  #endif
}

void send_time_to_clock(){
  
    Serial.println();
    delay(100);
    Serial.flush();
    delay(100);


  //ADD CRUDE TIMEZONE
  int tmph = timeClient.getHours();
  if((tmph+timezone)>23){
    tmph =tmph+timezone-24;
  }else if((tmph+timezone)<0){
    tmph = tmph+timezone+24;
  }else{
    tmph = tmph+timezone;
  }
  
  //UPDATE RTC IF USED
  rtc_hours = tmph;
  rtc_mins = timeClient.getMinutes();
  rtc_secs = 0;
  update_rtc();

  }



//ANTI BURN IN CYLCE ANIMATION
//LIKE REAL NIXIES
void start_abi(){
//LEGACY SUPPORT
              
   Serial.println("_abi_");
   delay(10);
   abi_counter = ABI_COUNTER_MAX;
   abi_started = true;
}


void loop_abi(){
  if(abi_started && abi_counter <= 0){
   abi_started = false;
   abi_counter = 0;
  }
  //UPDATE CLOCK DISPLAY
  abi_counter--;
  const int c = abi_counter % 10;
  update_clock_display(c,c,c,0,128,false);
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
    
    //LEGACY SEND TO ARDUINO BASED CLOCK
    Serial.flush();
    Serial.println();
    last_error = "_st_" + String(h) + "_" + String(m) + "_"+ String(s) + "_" + String(col) +"_" + String(_bright) +"_";
    Serial.println(last_error);          
    delay(100);
  
    //UPDATE NEOPIXEL
    //SPLIT HOURS MINS,.. INTO SEPERATE DIGITS
    const int m_tens = m / 10;      // tens now = 2
    const int m_ones = m % 10;      // ones now = 6 

    const int h_tens = h / 10;      // tens now = 2
    const int h_ones = h % 10;      // ones now = 6 
    
    const int s_tens = s / 10;
    const int s_ones = s % 10;
  
    pixels.clear();
    
    if(COUNT_CLOCK_DIGITS >= 2){
      pixels.setPixelColor(digit_offsets[0] + led_index_digits[h_tens], digit_color(h_tens,0,_disable_leading_zero, col, _bright));
      pixels.setPixelColor(digit_offsets[1] + led_index_digits[h_ones], digit_color(h_ones,1,_disable_leading_zero, col, _bright));
    }
 
    if(COUNT_CLOCK_DIGITS >= 4){
      pixels.setPixelColor(digit_offsets[2] + led_index_digits[m_tens], digit_color(m_tens,2,_disable_leading_zero, col, _bright));
      pixels.setPixelColor(digit_offsets[3] + led_index_digits[m_ones],digit_color(m_ones,3,_disable_leading_zero, col, _bright));
    }
 
    if(COUNT_CLOCK_DIGITS >= 6){
      pixels.setPixelColor(digit_offsets[4] + led_index_digits[s_tens], digit_color(s_tens,4,_disable_leading_zero, col, _bright));
      pixels.setPixelColor(digit_offsets[5] + led_index_digits[s_ones], digit_color(s_ones,5,_disable_leading_zero, col, _bright));
    }
    
   
    pixels.show();   // Send the updated pixel colors to the hardware.
 
}







void setup_mqtt_client(){
   if(sync_mode == 2 && mqtt_broker_url != "" && mqtt_broker_port != ""){
    client.setServer(mqtt_broker_url.c_str(), mqtt_broker_port.toInt());
    client.setCallback(mqtt_callback);
   }    
}

  
void handleSave()
{
    // PARSE ALL GET ARGUMENTS
    for (uint8_t i = 0; i < server.args(); i++) {
        // SNY MODE = enable ntc sync
        if (server.argName(i) == "sync_mode") {
            sync_mode = server.arg(i).toInt();
            if(sync_mode < 0 || sync_mode > 2){
              sync_mode = 0;
            }
            last_error = "set sync_mode to" + String(sync_mode);
        }
        // timezone +-12hours
        if (server.argName(i) == "timezone") {
            timezone = server.arg(i).toInt();
            if (timezone > 12 || timezone < -12) {
                timezone = 0;
            }
            last_error = "set timezone to" + String(timezone);
            //timeClient.setTimeOffset(timezone);
            timeClient.forceUpdate();
            send_time_to_clock();
        }
        // ntp_server_url
        if (server.argName(i) == "ntp_server_url") {
            ntp_server_url = server.arg(i);
            last_error = "set ntp_server_url to" + ntp_server_url;
            if(ntp_server_url != ""){
              timeClient.setPoolServerName(ntp_server_url.c_str());
              timeClient.forceUpdate();
            }
            last = 0;
        }

        // mqtt_broker_url
        if (server.argName(i) == "mqtt_broker_url") {
            mqtt_broker_url = server.arg(i);
            last_error = "set mqtt_broker_url to" + mqtt_broker_url;
            last = 0;
        }
       

        // mqtt_broker_port
        if (server.argName(i) == "mqtt_broker_port") {
            mqtt_broker_port = server.arg(i);
            last_error = "set mqtt_broker_port to" + mqtt_broker_port;
            last = 0;
        }

        // mqtt_broker_port
        if (server.argName(i) == "mqtt_topic") {
            mqtt_topic = server.arg(i);
            last_error = "set mqtt_topic to" + mqtt_topic;
            last = 0;
        }

        
        // formats the filesystem= resets all settings
        if (server.argName(i) == "fsformat") {
            if (SPIFFS.format()) {
                last_error = "Datei-System formatiert";
            }
            else {
                last_error = "Datei-System formatiert Error";
            }
        }
        //LOAD CURRENT SAVED DATA
        if (server.argName(i) == "eepromread") {
            restore_eeprom_values();
        }

        //LOAD FACOTRY RESET
        if (server.argName(i) == "factreset") {
           sync_mode = 1;
           timezone = DEFAULT_TIMEZONE;
           ntp_server_url = DEFAULT_NTP_SERVER;
           mqtt_broker_url = DEFAULT_MQTT_BROKER;
           mqtt_broker_port = DEFAULT_MQTT_BROKER_PORT;
           mqtt_topic = DEFAULT_MQTT_TOPIC;
           
           timeClient.forceUpdate();
        }

         //ANTI BURNOUT CYCLE
        if (server.argName(i) == "abi") {
            start_abi();
        }

      if (server.argName(i) == "sendtime") {
            send_time_to_clock();
            delay(100);
        }  
    }
    //SAVE THESE DATA
    save_values_to_eeprom();
    //SAVE MQTT STUFF 
    setup_mqtt_client();
    
    server.send(300, "text/html","<html><head><meta http-equiv='refresh' content='1; url=/' /></head><body>SAVE SETTINGS PLEASE WAIT</body></html>");
}






void handleRoot()
{



    String control_forms = "<hr><h2>DEVICE ID</h2>";
    control_forms+="<h1>" + String(MDNS_NAME) + String(ESP.getChipId()) + "</h1><br><br>";



  
    String timezonesign = "";
    if(timezone > 0){
      timezonesign = "+";
    }
   
 
     control_forms+="<hr><h2>CURRENT TIME</h2><h1>" + String(rtc_hours) + ":"+ String(rtc_mins) + ":" + String(rtc_secs) +"</h1>";
    control_forms+="<hr><h2>LAST NTP TIME</h2><h1>" + time_last + " ("+timezonesign+" "+ String(timezone)+" Hours)</h1>";



    control_forms += "<hr><h2>CONTROLS</h2>";
    control_forms += "<br><h3> MODE CONTROL </h3><br>";


        control_forms += "<form name='btn_on' action='/save' method='GET' required >"
                         "<select name='sync_mode' id='sync_mode'>";

                         if(sync_mode == 0){
                            control_forms += "<option value='0' selected>DISABLED</option>";
                         }else{
                            control_forms += "<option value='0'>DISABLED</option>";
                         }

                         if(sync_mode == 1){
                            control_forms += "<option value='1' selected>NTP-SYNC</option>";
                         }else{
                            control_forms += "<option value='1'>NTP-SYNC</option>";
                         }

                         if(sync_mode == 2){
                            control_forms += "<option value='2' selected>MQTT-SYNC</option>";
                         }else{
                            control_forms += "<option value='2'>MQTT-SYNC</option>";
                         }
                         
                     
        control_forms += "</select>"
                         "<input type='submit' value='SAVE'/>"
                         "</form>";
    
    


     control_forms += "<br><h3> CLOCK CONTROLS </h3>";
    
      control_forms += "<form name='btn_off' action='/save' method='GET'>"
                     "<input type='number' value='"+ String(timezone) + "' name='timezone' min='-12' max='12' required placeholder='1'/>"
                     "<input type='submit' value='SET TIMEZONE'/>"
                     "</form>";

    control_forms += "<br>";
    control_forms += "<br>";
    control_forms += "<br>";
    control_forms += "<form name='btn_off' action='/save' method='GET'>"
                     "<input type='text' value='"+ ntp_server_url + "' name='ntp_server_url' required placeholder='pool.ntp.org'/>"
                     "<input type='submit' value='SET NTP SERVER URL'/>"
                     "</form>";
                     
    control_forms += "<br>";
    

   control_forms += "<form name='btn_on' action='/save' method='GET' required >"
                     "<input type='hidden' value='sendtime' name='sendtime' />"
                     "<input type='submit' value='SEND NTP TIME TO CLOCK'/>"
                     "</form>";
                     



  control_forms += "<br><h3> MQTT SETTINGS </h3>";
  control_forms += "<form name='btn_offmq' action='/save' method='GET'>"
                     "<input type='text' value='"+ String(mqtt_broker_url) + "' name='mqtt_broker_url' required placeholder='broker.hivemq.com'/>"
                     "<input type='submit' value='SAVE MQTT BROKER'/>"
                     "</form>";

  control_forms += "<form name='btn_off' action='/save' method='GET'>"
                     "<input type='number' value='"+ String(mqtt_broker_port) + "' name='mqtt_broker_port' min='1' max='65536' required placeholder='1883'/>"
                     "<input type='submit' value='SET MQTT BROKER PORT'/>"
                     "</form>";
  control_forms += "<form name='btn_off' action='/save' method='GET'>"
                     "<input type='text' value='"+ String(mqtt_topic) + "' name='mqtt_topic' required placeholder='/iot/sensor/temp'/>"
                     "<input type='submit' value='SET MQTT TOPIC'/>"
                     "</form>";
   


     control_forms += "<br><h3> OTHER SETTINGS </h3>";

    control_forms += "<form name='btn_on' action='/save' method='GET' required >"
                     "<input type='hidden' value='eepromread' name='eepromread' />"
                     "<input type='submit' value='READ STORED CONFIG'/>"
                     "</form><br>";

    control_forms += "<form name='btn_on' action='/save' method='GET' required >"
                     "<input type='hidden' value='fsformat' name='fsformat' />"
                     "<input type='submit' value='DELETE CONFIGURATION'/>"
                     "</form><br>";
                     
    control_forms += "<form name='btn_on' action='/save' method='GET' required >"
                     "<input type='hidden' value='abi' name='abi' />"
                     "<input type='submit' value='START ANTI BURN IN CYCLE'/>"
                     "</form><br>";


                     
     control_forms += "<form name='btn_on' action='/save' method='GET' required >"
                     "<input type='hidden' value='factreset' name='factreset' />"
                     "<input type='submit' value='FACTORY RESET'/>"
                     "</form><br>";



    control_forms += "<br><hr><h3>LAST SYSTEM MESSAGE</h3><br>" + last_error;

    server.send(200, "text/html", phead_1 + WEBSITE_TITLE + phead_2 + pstart + control_forms + pend);
}



void handleNotFound()
{
    server.send(404, "text/html","<html><head>header('location: /'); </head></html>");
}






void setup(void)
{ 

   


    Serial.begin(SERIAL_BAUD_RATE);
    // START THE FILESYSTEM
    if (SPIFFS.begin()) {
        last_error = "SPIFFS Initialisierung....OK";
    }
    else {
        last_error = "SPIFFS Initialisierung...Fehler!";
    }



   

    
    // LOAD SETTINGS
    restore_eeprom_values();
    // START WFIFIMANAGER FOR CAPTIVE PORTAL
    WiFiManager wifiManager;
    wifiManager.setDebugOutput(false);
    wifiManager.autoConnect("LixieClockConfiguration");

    if (MDNS.begin((MDNS_NAME + String(ESP.getChipId())).c_str())) {
    }
    //WEBSERVER ROUTES
    delay(1000);
    server.on("/", handleRoot);
    server.on("/save", handleSave);
    server.on("/index.html", handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();
    
    //START OTA LIB
    ArduinoOTA.setHostname((MDNS_NAME + String(ESP.getChipId())).c_str());
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        }
        else { // U_SPIFFS
            type = "filesystem";
        }
        SPIFFS.end();
    });
    ArduinoOTA.onEnd([]() {});
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {});
    ArduinoOTA.onError([](ota_error_t error) {});
    ArduinoOTA.onEnd([]() {
        if (SPIFFS.begin()) {
            restore_eeprom_values(); // RESTORE FILE SETTINGS
        }
    });
    ArduinoOTA.begin();


    //SETUP  NTP CLIENT
    timeClient.setTimeOffset(timezone);
    timeClient.setPoolServerName(ntp_server_url.c_str());
    timeClient.begin();

    //SETUP MQTT
    setup_mqtt_client();
    
  
  //SETUP NEOPIXELS
  pixels.begin();
  pixels.clear();
  
  
  //RTC INIT
  #ifdef ENABLE_I2C_RTC
  Wire.begin();
  if (!rtc.begin()) {
    last_error = "_rct_begin_error_";
  }

  if (!rtc.isrunning()) {
    last_error = "RTC is NOT running!";
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }


  delay(100);
  DateTime now = rtc.now();
  rtc_hours = now.hour();
  rtc_mins = now.minute();
  rtc_secs = now.second();
  #endif
}



void loop(void)
{
    //HANDLE SERVER
    server.handleClient();
    //HANDLE NTP
    timeClient.update();
    time_last = timeClient.getFormattedTime();


    
    //UPDATE NTP NTP
    if ((millis() - last) > 1000 * 60 * NTP_SEND_TIME_INTERVAL) {
        last = millis();   
        if (sync_mode == 1) {
            send_time_to_clock();
        }
    }

      
     //MQTT RECONNECT FUNCTION
    if (!client.connected()) {
      mqtt_reconnect();
    }
    //MQTT LOOP FKT
    if(sync_mode == 2){
      client.loop();
    }
    
     //UPDATE RTC
    #ifdef ENABLE_I2C_RTC
    if (sync_mode == 1) {
      DateTime now = rtc.now();
      rtc_hours = now.hour();
      rtc_mins = now.minute();
      rtc_secs = now.second();
    }
    #endif
     
      
    //UPDATE CLOCK DISPLAY
    if (sync_mode == 1 && !abi_started) {
      update_clock_display(rtc_hours, rtc_mins, rtc_secs, map(rtc_secs,0,60,0,255), 255, false);
    }else if (sync_mode == 2 && !abi_started) {
      update_clock_display(mqtt_hours, mqtt_mins, mqtt_secs, map(mqtt_mins,0,99,0,255), 255, false);
    }else if(sync_mode == 0){
      update_clock_display(0, 0, 0, 0, 10, true);
    }else{
     
      //HANDLE ABI CYLCE
      if ((millis() - last_abi) > 1000) {
        last_abi = millis();   
        loop_abi();
      }
        
    }
 
   
 
    //HANDLE OTA
    ArduinoOTA.handle();
   
  
 
 
    
   
 
 
 
    delay(70);
}
