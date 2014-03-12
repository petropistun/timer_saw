/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.0 Professional
Automatic Program Generator
© Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 12.02.2014
Author  : NeVaDa
Company : 
Comments: 


Chip type               : ATtiny13
AVR Core Clock frequency: 1,000000 MHz
Memory model            : Tiny
External RAM size       : 0
Data Stack size         : 16
*****************************************************/

#include <tiny13.h>
#include <delay.h>


//pin0 pin1 - кнопки
//pin1 - старт
//pin0 - стоg
//PORTB.2 - діод
//PORTB.3 - релюшка, включається -
//PORTB.4 - релюшка, включається +


// Declare your global variables here
unsigned int time_unit = 0; //один юніт - 50 мс

#define MAX_TIME_IN_UTIN 15*60*20  // 15 min
#define ONE_MIN_TIME  1200  //одна хв

void Add50utint()  // 1 min
{
    if (time_unit < MAX_TIME_IN_UTIN)
    {            
        time_unit += ONE_MIN_TIME;
    }
}

void Start()
{
    time_unit = ONE_MIN_TIME; //одна хвилина
    PORTB.3 = 0; 
    PORTB.4 = 1;         
    PORTB.2 = 1;
}

void Stop()
{
    time_unit = 0;
    PORTB.3 = 1; 
    PORTB.4 = 0;
    PORTB.2 = 0;
}

#define TIME_BLICKING 5

void main(void)
{
// Declare your local variables here
unsigned char time_blick = TIME_BLICKING;

time_unit = 0;

// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=0x80;
CLKPR=0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif


// Input/Output Ports initialization
// Port B initialization
// Func5=Out Func4=Out Func3=Out Func2=Out Func1=In Func0=In 
// State5=1 State4=1 State3=0 State2=0 State1=P State0=P 
PORTB=0x33;
DDRB=0x3C;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0A output: Disconnected
// OC0B output: Disconnected
TCCR0A=0x00;
TCCR0B=0x00;
TCNT0=0x00;
OCR0A=0x00;
OCR0B=0x00;

// External Interrupt(s) initialization
// INT0: Off
// Interrupt on any change on pins PCINT0-5: Off
GIMSK=0x00;
MCUCR=0x00;

// Timer/Counter 0 Interrupt(s) initialization
TIMSK0=0x00;

// Analog Comparator initialization
// Analog Comparator: Off
ACSR=0x80;
ADCSRB=0x00;
DIDR0=0x00;

// ADC initialization
// ADC disabled
ADCSRA=0x00;

while (1)
      {
      // Place your code here           
      
      
        delay_ms(50);
        if (time_unit > 0) 
        {
            time_unit--;
            if (0 == time_unit)
            {         
                Stop();
            }
        }
        
        //мограння світодіода якщо менше хвилини          
        if (0 != time_unit && time_unit < ONE_MIN_TIME)
        {
            if (0 == time_blick)
            {
                PORTB.2 = !PORTB.2;
                time_blick = TIME_BLICKING;
            }             
            else
            {
                time_blick--;
            }
        }                 
        else
        {           
            if (0 == time_unit)
            {
                PORTB.2 = 0;
            }
            else
            {
                PORTB.2 = 1;
            }        
        }
                          
        
        //кнопка стоп
        if (0 == PINB.0)
        {
            Stop();
            while(0 == PINB.0) delay_ms(50); //чекаємо поки відпустять кнопку         
        }            
        
        //кнопка старт
        if (0 == PINB.1)
        {
            if (0 == time_unit)
            {
                Start();
                while(0 == PINB.1) delay_ms(50); //чекаємо поки відпустять кнопку            
            }           
            else
            {
                //додаємо одну хвилину
                Add50utint();
                
                while(0 == PINB.1) delay_ms(10); //чекаємо поки відпустять кнопку            

                //помограємо світодіодом - що хвилина додана
                PORTB.2 = 0;
                delay_ms(150);
                PORTB.2 = 1;  
                delay_ms(150);
                PORTB.2 = 0;
                delay_ms(150);
                PORTB.2 = 1;  
            }
        }

      }
}
