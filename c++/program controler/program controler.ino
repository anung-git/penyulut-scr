#include <LiquidCrystal.h>
//#define jumlahLubang    48
#define gatePin 7
//#define encoderPin 3
#define potensio A5
#define crossIn 2
//#define probe 11
#define PULSE 4   //trigger pulse width (counts)



#define setPin(b) ( (b)<8 ? PORTD |=(1<<(b)) : PORTB |=(1<<(b-8)) )
#define clrPin(b) ( (b)<8 ? PORTD &=~(1<<(b)) : PORTB &=~(1<<(b-8)) )
#define tstPin(b) ( (b)<8 ? (PORTD &(1<<(b)))!=0 : (PORTB &(1<<(b-8)))!=0 )



int average [5];
const int rs = 7, en = 6, d4 = 5, d5 = 10, d6 = 9, d7 = 8;

unsigned long previousMillis = 0;
const long interval = 500; 
int sudut;
volatile int PotValue ;


void interupsiPhaseCrossing(){
  TCCR1B=0x04; //start timer with divide by 256 input
  TCNT1 = 0;   //reset timer - count from zero 
}

ISR(TIMER1_COMPA_vect){ //comparator match
// digitalWrite(gatePin,HIGH);  //set TRIAC gate to high
  setPin(gatePin);
  TCNT1 = 65536-PULSE;      //trigger pulse width
}

ISR(TIMER1_OVF_vect){ //timer1 overflow
// digitalWrite(gatePin,LOW); //turn off TRIAC gate
  clrPin(gatePin);
  TCCR1B = 0x00;          //disable timer stopd unintended triggers
}

void setup(){
  Serial.begin(9600);
  pinMode(gatePin, OUTPUT);
  pinMode(potensio, INPUT);
  pinMode(crossIn, INPUT);
//  pinMode(encoderPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(crossIn), interupsiPhaseCrossing, FALLING);
  TIMSK1 =0x03;    //enable comparator A and overflow interrupts
  TCCR1A = 0x00;    //timer control registers set for
  TCCR1B = 0x00;    //normal operation, timer disabled  
}
void loop(){  
      PotValue = analogRead(potensio);
      OCR1A =1 + PotValue;
      delay(10);
//    unsigned long currentMillis = millis();
//    if (currentMillis - previousMillis >= interval) {
//      previousMillis = currentMillis;
//      for (byte i =0;i<4;i++){
//        average[i]=average[i+1];
//      }
//      average[4]=counterEncoder;
//      counterEncoder=0;
//      int result=0;
//      for (byte i =0;i<5;i++){
//        result = result + average[i];
//      }      
//      float RPM = result/2;// average * 2.5;//(counterEncoder/jumlahLubang)*(60000/interval);
//      
//      lcd.clear();
//      lcd.setCursor(0, 1);
//      lcd.print("RPM = ");
//      lcd.print(RPM);
//      sudut = map(PotValue, 0, 1000, 0, 180);
//      lcd.setCursor(0, 0);
//      lcd.print("SUDUT = ");
//      lcd.print(sudut);
//      Serial.print("rpm = ");
//      Serial.println(RPM);
//      Serial.print("sudut = ");
//      Serial.println(sudut);
//      Serial.print("ocr1a = ");
//      Serial.println(OCR1A);
//    }
}
//permission denied    sudo chmod a+rw /dev/ttyUSB0
//sudo avrdude -c usbasp -P usb -p atmega328p -U flash:w:program.ino.standard.hex
