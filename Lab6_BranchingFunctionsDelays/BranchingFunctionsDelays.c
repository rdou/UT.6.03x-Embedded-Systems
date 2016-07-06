// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

#include "TExaS.h"

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_CR           (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

typedef unsigned int  U32; 
typedef unsigned long U64; 

// basic functions defined at end of startup.s
void DisableInterrupts( void ); // Disable interrupts
void EnableInterrupts( void );  // Enable interrupts
void Delay100ms( U64 time );
void gpioInit( void );

int main( void )
{ 
    U64 volatile delay;
    U32 volatile swVal;
    TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz
    // initialization goes here

    EnableInterrupts();           // enable interrupts for the grader
    gpioInit();
    
    GPIO_PORTF_DATA_R = 0x4; 
    while( 1 )
    {
        Delay100ms( 1 ); 
        
        swVal = ( GPIO_PORTF_DATA_R & 0x10 ); 
        if( 0 == swVal ) 
        {
            GPIO_PORTF_DATA_R ^= 0x4;  
        }  
    }
}

void gpioInit( void )
{
    SYSCTL_RCGC2_R     = SYSCTL_RCGC2_GPIOF;
    GPIO_PORTF_CR      = (1 << 2) | (1 << 4); 
    GPIO_PORTF_AMSEL_R = 0; 
    GPIO_PORTF_PCTL_R  = 0; 
    GPIO_PORTF_AFSEL_R = 0;
    GPIO_PORTF_DIR_R   = (1 << 2); 
    GPIO_PORTF_PUR_R   = (1 << 4); 
    GPIO_PORTF_DEN_R   = (1 << 2) | (1 << 4); 
} 

void Delay100ms( U64 time )
{
    U64 i;
    
    while( time > 0 )
    {
        i = 1333333;  // this number means 100ms
        while( i > 0 )
        {
            i = i - 1;
        }
        time = time - 1; // decrements every 100 ms
    }
}
