// Now turn this trash into treasure!

#include <TM1637.h>

const int PIN_CLK = 2;
const int PIN_DIO = 3;
const int PIN_TRIG = 28;
const int PIN_ECHO = 27;
const int PIN_CLK_ROTARY = 8;
const int PIN_DT = 9;
const int PIN_SW = 10;

TM1637 numberCounter(PIN_CLK, PIN_DIO);

void setup() {
  
  // put your setup code here, to run once:
  Serial1.begin(115200);
  // Serial1.println("Hello, Raspberry Pi Pico W!");
  numberCounter.init();
  numberCounter.set(BRIGHT_TYPICAL);
  int last_CLK = HIGH;
  int calibration_value = 0;
  int unit_of_measure = 0; 
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_CLK_ROTARY, INPUT);
  pinMode(PIN_DT, INPUT);
  pinMode(PIN_SW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_CLK_ROTARY), readEncoder, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_SW), changeUnit, FALLING);
}


  int last_CLK = HIGH;
  int calibration_value = 0;
  int unit_of_measure = 0; // 0 is centimeter, 1 is inch
void readEncoder() {
  
  int last_DT = digitalRead(PIN_DT);
  if (last_DT == HIGH) {
    calibration_value ++;
    Serial1.println(calibration_value);
  } else if (last_DT == LOW) {
    calibration_value --;
    Serial1.println(calibration_value);
  }


  
}

void changeUnit() {
  
  if (unit_of_measure == 0) {
    unit_of_measure = 1;
  } else {
    unit_of_measure = 0;
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  int duration = pulseIn(PIN_ECHO, HIGH);
  int cm = duration / 58 + calibration_value;
  int inch = duration / 148 + calibration_value;
  int unitsOfMeasure[2] = {cm, inch};


  /*
  Serial1.print("Duration: ");
  Serial1.println(duration);
  Serial1.print("Distance in CM: ");
  Serial1.println(cm);
  Serial1.print("Distance in inches: ");
  Serial1.println(inch);
  */
  
  
  numberCounter.display(0, (unitsOfMeasure[unit_of_measure] / 1000) % 10);
  numberCounter.display(1, (unitsOfMeasure[unit_of_measure] / 100) % 10);
  numberCounter.display(2, (unitsOfMeasure[unit_of_measure] / 10) % 10);
  numberCounter.display(3, unitsOfMeasure[unit_of_measure] / 1 % 10);
  
  
  
  // Serial1.println(unitsOfMeasure[unit_of_measure]);
  
  // delay(100); // this speeds up the simulation
}