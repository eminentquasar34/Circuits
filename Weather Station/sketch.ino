// Now turn this trash into treasure!
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <bits/stdc++.h> 
using namespace std; 

#define DHT_TYPE DHT22

const float BETA = 3950;
const int PIN_TEMP = 26;
const int PIN_DHT = 28;
const int PIN_LDR = 22;
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int OLED_RESET = -1;
const int PIN_VERT = 20;
const int PIN_HORZ = 27;
const int PIN_SEL = 18;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(PIN_DHT, DHT_TYPE);


void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial1.println("Hello, Raspberry Pi Pico W!");
  pinMode(PIN_TEMP, INPUT);
  pinMode(PIN_DHT, INPUT);
  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_VERT, INPUT);
  pinMode(PIN_HORZ, INPUT);
  pinMode(PIN_SEL, INPUT_PULLUP);

  dht.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(7,5);
  display.println("Mini Weather Station");
  display.display();
  

}

float temperatureValues[3] = {0.0 , 0.0, 0.0};
const string temperatureStrings[3] = {" C", " F", " K"};


void loop() {
  // put your main code here, to run repeatedly:
  int lightOn = digitalRead(PIN_LDR);
  int vert = analogRead(PIN_VERT);
  int horz = analogRead(PIN_HORZ);
  bool selPressed = digitalRead(PIN_SEL);
  if (lightOn == LOW) {
    int analogValue = analogRead(PIN_TEMP);
    float celsius = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
    
    temperatureValues[0] = celsius;
    temperatureValues[1] = celsius * 9/5 + 32; // Fahrenheit
    temperatureValues[2] = celsius + 273.15;  // Kelvin
     

    /*
    Serial1.print("Temperature: ");
    Serial1.print(celsius);
    Serial1.println(" ℃");
    */
    float humidity = dht.readHumidity();
    
    if (isnan(humidity)) {
      Serial1.println("DHT sensor error");
      return;
    }
    display.clearDisplay();
    display.setCursor(2, 5);
    
    display.print("Temperature: ");
    if (horz > 700) {
      display.print(temperatureValues[1]);
      display.println(" F");
    } else if (horz < 300) {
      display.print(temperatureValues[2]);
      display.println(" K");
    } else {
      display.print(celsius);
      display.println(" C");
    }
    
    display.setCursor(2, 15);
    display.print("Humidity: ");
    display.print(humidity);
    display.println("%");
    display.display();
  } else {

    display.clearDisplay();
    display.setCursor(7,5);
    display.println("Mini Weather Station");
    display.display();
    
  
  
  }
  Serial1.println(horz);
  
  delay(10); // this speeds up the simulation
}