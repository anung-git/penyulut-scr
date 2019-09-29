/************************************************************* 
 It uses Timer0 or Timer0B, and divide the clock frequncy by 1024.
 The divided frequencys period is multiplied with the
 number og counts Timer0/Timer0B can take before it overflows.
 The number is then multiplied by 61, and gives approximately
 1 second.  
 16MHz / 1024 = 15625 hz        We divide the 16 MHz clock by 1024
 1/15625 = 0.000064s            Period time
 256 * 0.000064 = 0.016384      
 0.016384 * 61 = 0.999424 ≈ 1s   
 X = 0.999424 / 0.016384 where X is approximately 61
**************************************************************/ 
#include <avr/io.h>
#include <util/delay.h>

#define setPin(b) ( (b)<8 ? PORTD |=(1<<(b)) : PORTB |=(1<<(b-8)) )
#define clrPin(b) ( (b)<8 ? PORTD &=~(1<<(b)) : PORTB &=~(1<<(b-8)) )
#define setPinC(b) PORTC |=(1<<(b))
#define clrPinC(b) PORTC &=~(1<<(b))
#define setPinD(b) PORTD |=(1<<(b))
#define clrPinD(b) PORTD &=~(1<<(b))
#define setPinB(b) PORTB |=(1<<(b))
#define clrPinB(b) PORTB &=~(1<<(b))
//#define setPinABC(b) ( ((b)>13) ?   PORTC |=(1<<(b-13))  : ( (b)<8 ? PORTD |=(1<<(b)) : PORTB |=(1<<(b-8)) )  ) 

unsigned int timeCount;
volatile uint16_t counter;
#define com1  13
#define com2  12
#define com3  11
#define com4  10

void segOut(uint8_t s){
  for(byte i=0;i<6;i++){
    if (s>127){
      setPinC(i);
    }
    else{
      clrPinC(i);
    }
    s=s<<1;
  }
    if (s>127){
      setPinD(2);
    }
    else{
      clrPinD(2);
    }  
}

void scan(unsigned int value){
  uint8_t data1[]={3, 219, 133, 145, 89, 49, 33, 155, 1, 17};
  uint8_t data2[]={33, 237, 81, 69, 141, 7, 3, 109, 1, 5}; 
  uint8_t a=value/1000;
  uint8_t b=(value%1000)/100;
  uint8_t c=(value%100)/10;
  uint8_t d=value%10;
  segOut(data1[a]);
  setPin(com1);
 _delay_ms(1);
  clrPin(com1);
  segOut(data2[b]);
  setPin(com2);
   _delay_ms(1);
  clrPin(com2);
  segOut(data1[c]);
  setPin(com3);
   _delay_ms(1);
  clrPin(com3);
  segOut(data2[d]);
  setPin(com4);
   _delay_ms(1);
  clrPin(com4); 
}
int main (void){
  DDRB |= 0x20; //Set PB5 as output, ignore the rest
  #if defined(__AVR_ATmega8__)  
    TCCR0 = 0x05; // clock frequency / 1024 
    TCNT0 = 0; // Start to count from zero
    TIMSK = 0x01; // Enable overflow interrupt
    
    TCCR1B = 0X06;
  #else // ATmega48/P, ATmega88/P, ATmega168/P, ATmega328/P
    TCCR0B = 0x05; // clock frequency / 1024 
    OCR0B = 0x00;  // Output compare
    TCNT0 = 0; // Set counter 0 to zero
    TIMSK0 = 0x01; // Enable overflow interrupt
  #endif
  sei(); //Enable global interrupts
  DDRC=0xFF;
  DDRB=0x3C;
  DDRD=0x04;
  PORTB=0xFF;
  PORTB=0xFF;
  while(1){
//    int RPM;
//    RPM=counter * 1.25;
//    scan(RPM);
    scan(counter);
  }
}

ISR(TIMER0_OVF_vect){ //Timer 0 overflow vector - this run every time timer0 overflows
  timeCount++;
//  if(timeCount == 61) //Timer overflown for the 61th time
  if(timeCount == 76) //1.25 dt
  {
    counter = TCNT1;
    TCNT1=0;
    timeCount = 0;
  }
}
