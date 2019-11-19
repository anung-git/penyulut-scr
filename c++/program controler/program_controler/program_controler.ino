//#define jumlahLubang    48
#define gatePin 4
//#define encoderPin 3
#define potensio A5
#define crossIn 2
//#define probe 11
#define PULSE 4   //trigger pulse width (counts)



#define setPin(b) ( (b)<8 ? PORTD |=(1<<(b)) : PORTB |=(1<<(b-8)) )
#define clrPin(b) ( (b)<8 ? PORTD &=~(1<<(b)) : PORTB &=~(1<<(b-8)) )
#define tstPin(b) ( (b)<8 ? (PORTD &(1<<(b)))!=0 : (PORTB &(1<<(b-8)))!=0 )



int average [5];

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
  attachInterrupt(digitalPinToInterrupt(crossIn), interupsiPhaseCrossing, FALLING);
  TIMSK1 =0x03;    //enable comparator A and overflow interrupts
  TCCR1A = 0x00;    //timer control registers set for
  TCCR1B = 0x00;    //normal operation, timer disabled  
}
void loop(){  
      PotValue = analogRead(potensio);
      OCR1A =1 + (PotValue/1.6368);
      delay(10);

}
//permission denied    sudo chmod a+rw /dev/ttyUSB0
//sudo avrdude -c usbasp -P usb -p atmega328p -U flash:w:program.ino.standard.hex
