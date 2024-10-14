// Now turn this trash into treasure!
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int PIN_MOTION = 6;
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int OLED_RESET = -1;
int pirState = LOW;
int val = 0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  // put your setup code here, to run once:
  
  
  Serial1.begin(9600);
  Serial1.println("Hello, Raspberry Pi Pico W!");
  pinMode(PIN_MOTION, INPUT);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Motion Sensor");
  display.display();

}

void loop() {
  // put your main code here, to run repeatedly:
  // u8g.drawStr(25, 50, "Progress Bar");
  val = digitalRead(PIN_MOTION);
  if (val == HIGH) {
    if (pirState == LOW) {
      Serial1.println("motion detected!");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Motion Detected");
      display.display();
      pirState = HIGH;
      tone(8, 262, 250);
    }
  } else {
    if (pirState == HIGH) {
      Serial1.println("motion ended!");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("No Motion");
      display.display();
      pirState = LOW;
    }
  }

  
  delay(1); // this speeds up the simulation
}