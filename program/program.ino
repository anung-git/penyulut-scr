//#include "TimerOne.h"
#include <LiquidCrystal.h>
#define jumlahLubang    48

#define gatePin 4
#define encoderPin 3
#define potensio A0
#define crossIn 2
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;

unsigned long previousMillis = 0;
const long interval = 500; 
int sudut;
int PotValue ;
int wait;
volatile int counterEncoder;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void interupsiEncoder(){
    counterEncoder++;
}

void GateOn(){
//  Timer1.stop();
}

void interupsiPhaseCrossing(){
 // Timer1.restart();   //set our stopwatch to 0
 // delayMicroseconds(PotValue*10);
 // digitalWrite(gatePin,HIGH);
 //  delay(2);
 digitalWrite(gatePin,LOW);
 wait=PotValue*10;
 wait++;
 //  Timer1.start(); 
}

void setup(){
  pinMode(gatePin, OUTPUT);
  pinMode(potensio, INPUT);
  pinMode(crossIn, INPUT);
  pinMode(encoderPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(crossIn), interupsiPhaseCrossing, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderPin), interupsiEncoder, RISING);
 // Timer1.initialize(1000);  //longest pulse in is usally 20 milliseconds,
 // Timer1.attachInterrupt( GateOn ); 
 // Timer1.stop();            //stop the counter Timer1.restart();
  lcd.begin(16, 2);
  lcd.print("======ITNY======");
  lcd.clear();
}

void loop(){
  if(wait){
    delayMicroseconds(wait);
    wait=0;
    digitalWrite(gatePin,HIGH);
    delay(2);
    digitalWrite(gatePin,LOW);
  }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    float RPM = counterEncoder * 2.5;//((counterEncoder/48)*120);
    counterEncoder=0;
    lcd.setCursor(0, 1);
    lcd.print("RPM = ");
    lcd.print(RPM);
    lcd.setCursor(0, 0);
    lcd.print("SUDUT = ");
    PotValue = analogRead(potensio);
    //Timer1.setPeriod(PotValue*10);
    sudut = map(PotValue, 0, 1023, 0, 180);
    lcd.print(sudut);
  }

}
