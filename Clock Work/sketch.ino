// Now turn this trash into treasure!
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <Keypad.h>
#include <TM1637.h>
#include <iostream>
#include <string>
using namespace std;

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int OLED_RESET = -1;
const int PIN_CLK = 2;
const int PIN_DIO = 3;
const int PIN_MOTION = 28;
const int PIN_BUZZ = 8;
int pirState = LOW;

int oldValue = LOW;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS1307 rtc;
TM1637 numberCounter(PIN_CLK, PIN_DIO);




// Alarm
int alarm_hour = 6; //Army time
int alarm_minute = 30;
int alarm_sound_length = 0;
bool alarm_initialized = false;
bool motion_sensed = false;
int val = 0;


void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");
  numberCounter.init();
  numberCounter.set(BRIGHT_TYPICAL);
  numberCounter.point(POINT_ON);
  pinMode(PIN_MOTION, INPUT);
  if (! rtc.begin()) {
    Serial1.println("Couldn't find RTC");
    Serial1.flush();
    abort();
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial1.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(7,5);
  display.println("ClockWork");
  display.setCursor(7, 15);
  display.print("Alarm: ");
  display.print(alarm_hour);
  display.print(":");
  if (alarm_minute < 10) {
    display.print("0");
    display.println(alarm_minute);
  } else {
    display.println(alarm_minute);
  }
  display.display();
}


void loop() {
  // put your main code here, to run repeatedly:
  
  DateTime now = rtc.now();
  int hour = now.twelveHour();
  int actualHour = now.hour();
  int minute = now.minute();
  int second = now.second();
  int pm = now.isPM(); // 0 if AM, 1 if PM
  

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(7,5);
  display.println("ClockWork");
  display.setCursor(7, 15);
  display.print("Alarm: ");
  display.print(alarm_hour);
  display.print(":");
  if (alarm_minute < 10) {
    display.print("0");
    display.println(alarm_minute);
  } else {
    display.println(alarm_minute);
  }

  display.setCursor(7, 40);
  if (pm == 0) {
    display.println("AM") ;
  } else {
    display.println("PM");
  }
  display.display();


  numberCounter.display(0, hour / 10);
  numberCounter.display(1, hour % 10);
  numberCounter.display(2, minute / 10);
  numberCounter.display(3, minute % 10);
  if (actualHour == alarm_hour && minute == alarm_minute) {
    if (second < 2) {
      alarm_initialized = true;
    }
    if (alarm_sound_length < 40 && !motion_sensed && alarm_initialized) {
      val = digitalRead(PIN_MOTION);  // read input value
      if (val == HIGH) {            // check if the input is HIGH
        if (pirState == LOW) {
          motion_sensed = true;
          // We only want to print on the output change, not state
          pirState = HIGH;
        } 
      } else {
        if (pirState == HIGH) {
          // we have just turned of
          // We only want to print on the output change, not state
          motion_sensed = false;
          pirState = LOW;
          alarm_initialized = false;
        }
      }
      Serial1.println("alarm tone sounded");
      tone(8, 300, 100);
      alarm_sound_length ++;
    }
    }
  

  delay(1000); // this speeds up the simulation
}




