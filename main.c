/*
 */

#include <xc.h>
#include <avr/interrupt.h>

//Define all Ports&Pins for LEDs
#define LED_10_PIN PORTB0
#define LED_10_ON PORTB |= (1 << LED_10_PIN)
#define LED_10_OFF PORTB &= ~(1 << LED_10_PIN)

#define LED_9_PIN PORTD7
#define LED_9_ON PORTD |= (1 << LED_9_PIN)
#define LED_9_OFF PORTD &= ~(1 << LED_9_PIN)

#define LED_8_PIN PORTD6
#define LED_8_ON PORTD |= (1 << LED_8_PIN)
#define LED_8_OFF PORTD &= ~(1 << LED_8_PIN)

#define LED_7_PIN PORTD5
#define LED_7_ON PORTD |= (1 << LED_7_PIN)
#define LED_7_OFF PORTD &= ~(1 << LED_7_PIN)

#define LED_6_PIN PORTD4
#define LED_6_ON PORTD |= (1 << LED_6_PIN)
#define LED_6_OFF PORTD &= ~(1 << LED_6_PIN)

#define LED_5_PIN PORTC5
#define LED_5_ON PORTC |= (1 << LED_5_PIN)
#define LED_5_OFF PORTC &= ~(1 << LED_5_PIN)

#define LED_4_PIN PORTC4
#define LED_4_ON PORTC |= (1 << LED_4_PIN)
#define LED_4_OFF PORTC &= ~(1 << LED_4_PIN)

#define LED_3_PIN PORTC3
#define LED_3_ON PORTC |= (1 << LED_3_PIN)
#define LED_3_OFF PORTC &= ~(1 << LED_3_PIN)

#define LED_2_PIN PORTC2
#define LED_2_ON PORTC |= (1 << LED_2_PIN)
#define LED_2_OFF PORTC &= ~(1 << LED_2_PIN)

#define LED_1_PIN PORTC1
#define LED_1_ON PORTC |= (1 << PORTC1)
#define LED_1_OFF PORTC &= ~(1 << PORTC1)

#define BUTTON_PRESSED !(PIND & (1 << PIND1))


void init(void) {
    
    //LED
    //Configure Pins 1-5 of Port C as Output
    DDRC |= (1 << DDC1) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3) | (1 << DDC4) | (1 << DDC5);
  
    //Configure Pin 4-7 of Port D as Output
    DDRD |= (1 << DDD4);
    DDRD |= (1 << DDD5);
    DDRD |= (1 << DDD6);
    DDRD |= (1 << DDD7);
    
    //Configure Pin 0 of Port B as Output
    DDRB |= (1 << DDB0);
    
    
    //BUTTON
    //Configure Button1 as Input
    DDRD &= ~(1 << DDD1); 
    
    PORTD |= (1 << PORTD1);    //Enable Pull-Up
    
    PCICR |= (1 << PCIE2); //Enable PC Interrupt on Port D                                                                    
    PCMSK2 |= (1<<PCINT17); //Enable PC Interrupt on Pin 1 on PD
 
    
    //TIMER
    TIMSK0 |= (1 << OCIE0A); //Enable Timer0 Compare Match A Interrupt //Overflow Bit
    
    //Set CTC Mode 
    TCCR0A |= (1 << WGM01); 
    TCCR0A &= ~(1 << WGM00); 
    TCCR0B &= ~(1 << WGM02); 
    
    OCR0A = 250; //Timer Match A after 1000ms
    
    //Set Prescaler at 64 and start Timer
    TCCR0B |= (1 << CS00) | (1 << CS01);
    TCCR0B &= ~(1 << CS02); 
    
}

ISR(TIMER0_COMPA_vect) {
    
    static volatile uint8_t debounceCounter = 0;
    
    static volatile uint8_t currentStateOfButton = 0;
    static volatile uint8_t lastStateOfButton = 0;
 
    if(BUTTON_PRESSED) {
        
        if(debounceCounter == 20) { //Button is pressed for the length of 20ms 
            currentStateOfButton = 1; 
        } else {
            debounceCounter++; 
        }
     
    } else { //Button is not pressed or pressed for too short a duration
        
        debounceCounter = 0; 
        currentStateOfButton = 0; 
        
    } 
    
    if(lastStateOfButton == 0 && currentStateOfButton == 1) { //button held stable in pressed state 
        
        switchLED();
        
    }
    
    lastStateOfButton = currentStateOfButton; //update
     
}

void switchLED(void) {
    
    static volatile uint8_t currentLED = 0;
    currentLED++;
    
    switch(currentLED) { 
        
        case 1:
            LED_10_OFF;
            LED_1_ON;
            break;
        case 2:
            LED_1_OFF;
            LED_2_ON;
            break;
        case 3:
            LED_2_OFF;
            LED_3_ON;
            break;
        case 4:
            LED_3_OFF;
            LED_4_ON;
            break;
        case 5:
            LED_4_OFF;
            LED_5_ON;
            break;
        case 6:
            LED_5_OFF;
            LED_6_ON;
            break;
        case 7:
            LED_6_OFF;
            LED_7_ON;
            break;
        case 8:
            LED_7_OFF;
            LED_8_ON;
            break;
        case 9:
            LED_8_OFF;
            LED_9_ON;
            break;
        case 10:
            LED_9_OFF;
            LED_10_ON;
            currentLED = 0; //Reset
            break;
            
        }
    
}

void main(void) {
    
    sei(); //Enable Global Interrupt Bit 
    
    init();
    
    while(1) {
    }
            
}
