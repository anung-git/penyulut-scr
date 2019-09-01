#include "TimerOne.h"
#include <LiquidCrystal.h>

#define jumlahLubang    48

#define gatePin 3
#define encoderPin 4
#define potensio A0
#define crossIn 3

unsigned long previousMillis = 0;
const long interval = 500; 
int sudut;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

volatile int counterEncoder;

void interupsiEncoder(){
    counterEncoder++;
}

void GateOn(){
  Timer1.stop();
  digitalWrite(gatePin,HIGH);
  _delay_us(100);
  digitalWrite(gatePin,LOW);
    
}

void interupsiPhaseCrossing(){
  Timer1.restart();   //set our stopwatch to 0
  Timer1.start(); 
}

void setup(){
  pinMode(gatePin, OUTPUT);
  pinMode(potensio, INPUT);
  pinMode(crossIn, INPUT);
  pinMode(encoderPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(crossIn), GateOn, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderPin), interupsiEncoder, RISING);
  Timer1.initialize(20000);  //longest pulse in is usally 20 milliseconds,
  Timer1.stop();            //stop the counter Timer1.restart();
  lcd.begin(16, 2);
  lcd.print("======ITNY======");
}

void loop(){
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

    int sensorValue = analogRead(potensio);
    sensorValue = map(sensorValue, 0, 1023, 0, 180);
    if(sensorValue!=sudut){
      sudut=sensorValue;
      lcd.print(sudut);
      Timer1.initialize(1+sudut);
    }
  }

}
