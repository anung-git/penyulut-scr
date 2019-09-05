#include <LiquidCrystal.h>
#define jumlahLubang    48
#define gatePin 4
#define encoderPin 3
#define potensio A5
#define crossIn 2
#define probe 11
#define PULSE 70   //trigger pulse width (counts)
const int rs = 7, en = 6, d4 = 5, d5 = 10, d6 = 9, d7 = 8;

unsigned long previousMillis = 0;
const long interval = 500; 
int sudut;
volatile int PotValue ;
volatile int counterEncoder;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void interupsiEncoder(){
    counterEncoder++;
}

void interupsiPhaseCrossing(){
  digitalWrite(gatePin,LOW);
  digitalWrite(probe,LOW);
  TCCR1B=0x04; //start timer with divide by 256 input
  TCNT1 = 0;   //reset timer - count from zero 
}

ISR(TIMER1_COMPA_vect){ //comparator match
  digitalWrite(gatePin,HIGH);  //set TRIAC gate to high
  digitalWrite(probe,HIGH);
  TCNT1 = 65536-PULSE;      //trigger pulse width
}

ISR(TIMER1_OVF_vect){ //timer1 overflow
  digitalWrite(probe,LOW); //turn off TRIAC gate
  TCCR1B = 0x00;          //disable timer stopd unintended triggers
}

void setup(){
  pinMode(gatePin, OUTPUT);
  pinMode(probe, OUTPUT);
  pinMode(potensio, INPUT);
  pinMode(crossIn, INPUT);
  pinMode(encoderPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(crossIn), interupsiPhaseCrossing, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoderPin), interupsiEncoder, RISING);
  TIMSK1 =0x03;    //enable comparator A and overflow interrupts
  TCCR1A = 0x00;    //timer control registers set for
  TCCR1B = 0x00;    //normal operation, timer disabled  
  lcd.begin(16, 2);
  lcd.print("      ITNY      ");
  lcd.setCursor(0, 1);
  lcd.print("   YOGYAKARTA   ");
  lcd.clear();
}
void loop(){  
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      float RPM = counterEncoder * 2.5;//(counterEncoder/jumlahLubang)*(60000/interval);
      counterEncoder=0;
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("RPM = ");
      lcd.print(RPM);
      PotValue = analogRead(potensio);
      OCR1A =1 +( PotValue/1.6 );
      sudut = map(PotValue, 0, 1000, 0, 180);
      lcd.setCursor(0, 0);
      lcd.print("SUDUT = ");
      lcd.print(sudut);
    }
}
