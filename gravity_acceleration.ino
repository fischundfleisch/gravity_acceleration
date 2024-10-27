/**************************************************************************
  TTGO T-Display ST7789 OLED based on Adafruit example
  https://github.com/adafruit/Adafruit-ST7735-Library/blob/master/examples/graphicstest/graphicstest.ino


  Gravity Tester by Marlene Kaser
  v1.1 - Release date 20.10.2024
  follow my projects on github: https://github.com/fischundfleisch

  Two IR sensors are mounted at 1 meter distance. A small electromagnet is used to hold an object and release it
  when a button is pressed. The ESP32 TTGO T1 measures the time passed and calculates g

  buttons on ESP attached to PIN 0 and PIN 35
 **************************************************************************/
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

// pinouts from https://github.com/Xinyuan-LilyGO/TTGO-T-Display
#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_CS 5
#define TFT_DC 16
#define TFT_RST 23
#define TFT_BL 4

const int START_BUTTON = 0;

const int IR_TOP = 27;
const int IR_BOTTOM = 15; // must be checked and changed

// constructor for data object named tft
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup(void) {
  Serial.begin(115200); Serial.print(F("Hello! ST77xx TFT Test"));
  pinMode(TFT_BL, OUTPUT);      // TTGO T-Display enable Backlight pin 4
  pinMode(IR_TOP, INPUT);
  pinMode(START_BUTTON, INPUT);
  digitalWrite(START_BUTTON, HIGH);

  digitalWrite(TFT_BL, HIGH);   // T-Display turn on Backlight
  tft.init(135, 240);           // Initialize ST7789 240x135
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1, 3);
  tft.println("System Standby");
delay(1000);
  Serial.println(F("Initialized"));
}

void loop() {
  uint16_t stoptime = 0;
  uint16_t starttime = 0;
  uint16_t timetotal = 0;

  int is_there_top = digitalRead(IR_TOP);            // checks if something is present at the top IR sensor. 
  int is_there_bottom = digitalRead(IR_BOTTOM);
  int start_program = digitalRead(START_BUTTON);
//  Serial.println(digitalRead(START_BUTTON));


  if (start_program == false) {
  Serial.println("Ready");
  tft.println("System ready");
    while (digitalRead(IR_TOP) == false);             // do nothing while this is false: while two leds are on at the sensor, the program does nothing. Only when the object is dropped and only one led is on, the code resumes.
      starttime = millis();                           // sensor state has changed, so we take a timestamp and save the time the object passes the sensor
      Serial.println(starttime);

      while (digitalRead(IR_BOTTOM) == true);         // this means: do nothing until IR_BOTTOM becomes true
      stoptime = millis();                            // this happens when IR_BOTTOM is true. It saves the time at which IR_BOTTOM became true
      Serial.println(stoptime);
      timetotal = stoptime - starttime;               // substracts the starttime from the endtime and tells us how many milliseconds have passed
      Serial.println(timetotal);                      // prints out the milliseconds on Serial Monitor

float timeseconds = timetotal/1000.;
    tft.print("Dauer: ");
    tft.println(timeseconds);
  
    // g = 2/(timetotal²)
  
    float gravity;
    gravity = 2./(timeseconds*timeseconds);
    Serial.print("Fallbeschleunigung: ");
    Serial.println(gravity);
    tft.print("g = ");
    tft.println(gravity);
  }
}
