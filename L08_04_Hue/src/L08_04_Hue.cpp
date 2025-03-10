/* 
 * Project midterm project
 * Author: Ben Njus
 * Date: 03/10/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "IoTClassroom_CNM.h"
#include "encoder.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "neopixel.h"
#include "colors.h"
#include "wire.h"
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char logo16_glcd_bmp[] =
{ 0B00000000, 0B11000000,
  0B00000001, 0B11000000,
  0B00000001, 0B11000000,
  0B00000011, 0B11100000,
  0B11110011, 0B11100000,
  0B11111110, 0B11111000,
  0B01111110, 0B11111111,
  0B00110011, 0B10011111,
  0B00011111, 0B11111100,
  0B00001101, 0B01110000,
  0B00011011, 0B10100000,
  0B00111111, 0B11100000,
  0B00111111, 0B11110000,
  0B01111100, 0B11110000,
  0B01110000, 0B01110000,
  0B00000000, 0B00110000 };


SYSTEM_MODE(SEMI_AUTOMATIC); //control logging into classroom router


//SYSTEM_THREAD(ENABLED); Caution - will allow code to execute before fully connected

const int BULB=5; //number of bulb at table
const int PIXELCOUNT = 16; //number of neopixels
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);
const int NeopixelPin = D2; //Neopixel Pin output
int color; //color of neopixel
int n; //which neopixel
bool switchState; 
bool prevSwitchState;
const int SWPIN = D12; //output of encoder
const int PINA = D8;
const int PINB = D9;
Encoder myEnc(PINA, PINB);
const int maxPos = 95;
const int minPos = 0;
int position; //position of encoder
const int brightness = 255; // brightness of neopixels
const int LEDBLUE = D11;
const int LEDGREEN = D13;
const int LEDRED = D14;
const int MYWEMO=3;//Wemo device at table3
const int MYWEMO=4; //Wemo device at my table
float tempC; 
float humidRH;
const int hexAddress = 0x76;//hex address for BME sensor
bool status;//status OLED
const int OLED_RESET=-1;
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_BME280 bme;

void setup() {
  pinMode(SWPIN,INPUT_PULLDOWN); //setup Encoder Switch
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDBLUE, OUTPUT);
  pinMode(NeopixelPin, OUTPUT);// setup Neopixel OutPut
  pixel.begin();
  //pixel.setPixelColor(n, color); //set pixel color
  pixel.setBrightness(bri);
  pixel.show(); //initialize all off
  Serial.begin(9600);
  status = bme.begin(hexAddress); 
  waitFor(Serial.isConnected,10000);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  if (status == false) {
  Serial.printf("BME280 at address 0x76 failed to start"); }
  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.printf("Hello, world!\n");
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
  display.display();
  display.clearDisplay();
  Wire.begin();//data transmission
  Wire.beginTransmission(i);
  Wire.endTransmission();
  WiFi.on();//WiFi on
  WiFi.clearCredentials();
  WiFi.setCredentials("IoTNetwork");
  WiFi.connect();
}

 void loop() {
  switchState = digitalRead(SWPIN);
  if(switchState == 1){
      wemoWrite(MYWEMO,HIGH);
      wemoWrite(MYWEMO,HIGH);
      digitalWrite (LEDGREEN, LOW);
      setHue(BULB,true,HueRainbow[color%7],random(32,255),255);
      //color++;
      tempC = bme.readTemperature(); // read deg C
      humidRH = bme.readHumidity(); //read in humidity
      if (tempC > 20.0){
        for(int i = 0; i < 15, i++){
            pixel.setPixelColor(i,red);
            pixel.show(); 
            digitalWrite (LEDRED, HIGH);  
              }
      }
      if (tempC <= 20.0){
        for(int i = 0; i < 15, i++){
          pixel.setPixelColor(i,blue);
          pixel.show(); 
          digitalWrite (LEDBLUE, HIGH);
          }
       }
      if (humidRH > 25.0){
        for (int rot = 0; rot < 4; rot++){
          display.setRotation(rot);
          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(WHITE);
          display.setCursor(0,0);
          display.printf("Hello, world!\n");
          display.printf("Se%cor Ben Njus\n", enna);
          display.printf("it's Humid!\n"); 
          display.display();
          delay(2000); 
      }
      if (humidRH <= 25.0) {
        for (rot = 0; rot < 4; rot++){
          display.setRotation(rot);
          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(WHITE);
          display.setCursor(0,0);
          display.printf("Hello, world!\n");
          display.printf("Se%cor Ben Njus\n", enna);
          display.printf("It feels nice out\n"); 
          display.display();
          delay(2000); 
      }
  }
  if (switchState == 0) {
        position = myEnc.read(); 
        if (position < 0){
            myEnc.write(minPos);
        }
        if (position > 95){
            myEnc.write(maxPos);
        }
        brightness = round(2.68*position);
        setHue(BULB,true,HueRainbow[color%7],brightness,255);
        n = round(position*.157);
        pixel.setPixelColor(n, green);
        pixel.show();
        display.display();
        delay(100);
        display.clearDisplay(); 
    } 
}


 
    