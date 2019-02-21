#include <LiquidCrystal.h>
const int pLCD_RS = 10;
const int pLCD_EN = 9;
const int pLCD_DB4 = 5;
const int pLCD_DB5 = 4;
const int pLCD_DB6 = 3;
const int pLCD_DB7 = 2;
const int sensorPin = A0;
int angle;
LiquidCrystal LCD(pLCD_RS, pLCD_EN, pLCD_DB4, pLCD_DB5, pLCD_DB6, pLCD_DB7);

int trigPin = 13;
int echoPin = 11;
int myCounter = 0;
int servoControlPin = 6;
float pingTime; //Time for pin to travel from sensor to the target and rebound back
float targetDistance; //Distance to target in meters
float speedOfSound = 343; //Speed of sound in meters per second

void setup() {

  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(sensorPin, INPUT);
  LCD.begin(16,2); //Tell arduino to start our 16 column and 2 row LCD
  LCD.setCursor(0,0); // Set cursor to top left corner
}

void loop() {

 digitalWrite(trigPin, LOW); //Set the trigger pin to LOW
 delayMicroseconds(2000); //Let the signal settle
 digitalWrite(trigPin, HIGH); //set trigPin to HIGH
 delayMicroseconds(15); //Delay in the high state
 digitalWrite(trigPin, LOW); //Ping has now been sent
 delayMicroseconds(10); // Delay in the HIGH state
 angle = map(analogRead(sensorPin),191,875,180,0);

 pingTime = pulseIn(echoPin, HIGH); //pinTime in represented in microseconds
 pingTime = pingTime/1000000; //PingTime converted to seconds from microseconds
 targetDistance = speedOfSound * pingTime; //Will be in meters 
 targetDistance = targetDistance/2; //Because ping travels to the target and back, so you must divide for the actual distance
 

if (targetDistance <= 0.4 && targetDistance >=0.05){
  LCD.print("Distance:");
  LCD.print(targetDistance);
  LCD.print("m");
  LCD.setCursor(0,1);
  LCD.print("Angle:");
  LCD.print(angle);
  LCD.print(char(223));
}
else {
 LCD.print("Out of range");
}
delay(300);
LCD.clear();
}
