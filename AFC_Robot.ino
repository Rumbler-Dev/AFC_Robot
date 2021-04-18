/*******************************************AFC ROBOT*******************************************/
/*-----------------------------------------TECH RAMBLER-----------------------------------------*/

#include <MD_MAX72xx.h>           //Dot matrix library
#include <SPI.h>
#include "MD_EyePair.h"
#include <LiquidCrystal.h>       //LCD library
#include <dht.h>                 //DHT11 library

#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 10
#define rs 12                 //LCD reset pin
#define en 11                 //LCD enable pin
#define d4 5                  //LCD digital pin 
#define d5 4                  //LCD digital pin 
#define d6 3                  //LCD digital pin 
#define d7 2                  //LCD digital pin
#define relay 10              //relay pin
#define MQ2pin 6              //MQ2 sensor pin
#define DHTpin 8              //DHT11 sensor pin
#define CLK_PIN   52          // or SCK
#define DATA_PIN  51          // or MOSI
#define CS_PIN    53          // or SS
#define motorP 22             //motor +ve pin
#define motorN 23             //motor -ve pin 
#define shortdelay 5000
#define longdelay 15000
#define MAX_EYE_PAIR (MAX_DEVICES/2)

//Initialization
dht DHT;
int MQ2value = 1;
int DHTvalue = 0;
int state = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void stp();
void fwd();
void back();
void dhtdisplay();
MD_MAX72XX M = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
MD_EyePair E[MAX_EYE_PAIR];

//declaration section
void setup() {
  M.begin();
  pinMode(relay, OUTPUT);
  pinMode(rs, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(d5, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d7, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  pinMode(motorP, OUTPUT);
  pinMode(motorN, OUTPUT);
  pinMode(MQ2pin, INPUT);
  pinMode(DHTpin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);            //LCD initialization

  //Dot matrix initialization
  for (uint8_t i = 0; i < MAX_EYE_PAIR; i++)
    E[i].begin(i * 2, &M, 15);
}

//loop section
void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Tech Rambler");
  delay(2000);
  int chk = DHT.read11(DHTpin);      //Reading DHT11 sensor value
  MQ2value = digitalRead(MQ2pin);    // Reading MQ2 sensor value

  //check wheather the CO2 density is more
  if (MQ2value == 0) {
    digitalWrite(relay, HIGH);       //If the CO2 density is more turn ON the air purification
    lcd.setCursor(0, 0);
    lcd.print("Purification ON");   //Display the toast
    delay(1000);
    lcd.clear();
    dhtdisplay();                  //Display temprature and humidity of current room
  }
  else {
    digitalWrite(relay, LOW);      //If the CO2 density is more turn OFF the air purification
    lcd.setCursor(0, 0);
    lcd.print("Purification OFF"); //Display the toast
    delay(1000);
    lcd.clear();
    dhtdisplay();                 //Display temprature and humidity of current room
  }

  for (uint8_t i = 0; i < MAX_EYE_PAIR; i++)  //Display eye animation
    E[i].animate();

  fwd();                    //Move Forward
  delay(shortdelay);
  back();                  //Move Backward
  delay(shortdelay);
  fwd();                   //Move Forward
  delay(shortdelay);
  back();                //Move Backward
  delay(shortdelay); 
  stp();                  //Stop
  delay(longdelay);
}

//Move forward function
void fwd() {
  digitalWrite(motorP, HIGH);
  digitalWrite(motorN, LOW);
}

//Move backward function
void back() {
  digitalWrite(motorP, LOW);
  digitalWrite(motorN, HIGH);
}

//Stop the robot
void stp() {
  digitalWrite(motorP, LOW);
  digitalWrite(motorN, LOW);
}

//Displaying temprature and Humidity value
void dhtdisplay() {
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(DHT.humidity);
  lcd.print("%");
  delay(1000);
}
