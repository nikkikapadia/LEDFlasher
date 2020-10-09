// cTester.c
/*********************************************************************
Module:
main()
Author - Nikki Kapadia
Date - June 3 2019
Description : Program turns on LED lights
Hardware Notes:
 * PIC used
 * eg. PIC16F684 operating at 4 MHz, Tosc = 0.25 us
 * I/O ports used and hardware attached
 * eg. Output:
 RA1 - pin 12 connected to LED
 RA2 - pin 11 connected to LED
 RA5 - pin 2 connected to LED
 RC0 - pin 10 connected to LED
 RC1 - pin 9 connected to LED
 RC2 - pin 8 connected to LED
 RC3 - pin 7 connected to LED
 RC4 - pin 6 connected to LED

 Input : Two switches on pin 13 (RA0) and pin 3 (RA4)
 ********************************************************************/
/*******************************************************************
Include Files
 ********************************************************************/
#include "pic16f684.h"
#include "xc.h"
/*******************************************************************
Symbolic Constants used by main()
 ********************************************************************/
#define TRUE 1
#define FALSE 0
#define ON 0
#define OFF 1
#define SWITCH0 RA0
#define SWITCH1 RA4
#define LED1 0b000001
#define LED2 0b000100
#define LED3 0b010000
#define LED4 0b001000
#define LED5 0b100000
#define LED6 0b000010
#define LED7 0b000100
#define LED8 0b000010
// _XTAL_FREQ must be defined for __delay_ms() macro
#define _XTAL_FREQ 4000000
/*******************************************************************
Local Function Prototypes
 ********************************************************************/
void Initialize(void); // Initialize peripheral SFRs and global variables
void msDelay(unsigned count); // Call the assembler __delay_ms macro
void startingPattern(int time); // calls the starting pattern to flash the LEDs on and off
void patternOne (void); // calls the method with the various methods to make up the first pattern
void patternTwo (void); // calls the method with the various methods to make up the second pattern
void turnOnLEDs(void); // calls method to just turn on LEDs
void turnOffLEDs (void); // calls method to turn off all LEDS
void clockwiseCircle(void); // calls method to make LEDs turn on in a clockwise circle pattern
void counterClockwiseCircle(void); // calls method to make LEDs turn on in a counter clockwise circle pattern
void twoHalfCircles (void); // calls method to make LEDs turn on in pattern the turns on two half circles at the same time
void circleOn (int time); // turn on the full circle in the pattern of the two half circles
void circleOff (int time); // turn off the full circle in the pattern of the two half circles
void oppositesOnCircleCCW(void); // turn on opposite LEDs while going in a ccw direction
void oppositesOnCircleCW (void); // turn on opposite LEDs while going in a cw direction
void triangleTurnCW(void); // turn on LEDs in a triangle shape and rotates cw
void squareTurnCW (void); // turn on LEDs in a square shape and rotates cw
void quickFlashOpposites (void); // quickly flashes the LEDs opposite of each-other 3 times per pair
void delayFlash(void); // all lights flash on and the delay decreases each time
void circleFill(void); // fills the circle in a clockwise matter
Void circleEmpty (void); // clears the circle in a clockwise matter

/*** Configuration Bit Settings *************************************************
 * WDTE : Watch Dog Timer, PWRTE : Power Up Timer, CP : Code Protection, BOREN : Brown Out Detect
 * FCMEN : Fail Safe Clock Monitor, MCLRE : MCLR Pin Function (pin 4), CPD : Data Code Protect
 * IESO : Internal External Switchover mode,
 * FOSC : Oscillator selection - HS : High Speed, LP : Low Power, XT : External crystal oscillator
 * - INTOSCIO : Internal oscillator and I/O function on pin 4
 */
#pragma config WDTE=0,PWRTE=0,CP=1,BOREN=0,FCMEN=0,MCLRE=0,CPD=1,IESO=0,FOSC=INTOSCIO
/*******************************************************************
Global Variable Declarations
 ********************************************************************/
int count;

/*******************************************************************
main() function
 ********************************************************************/
int main(void) {
    Initialize(); // Initialize the PIC hardware
    while (TRUE) { //loop forever
        // Sample code
        if (SWITCH0 == OFF && SWITCH1 == OFF) {
            turnOffLEDs();
        }
        if (SWITCH0 == ON && SWITCH1 == OFF) {
            patternOne();
        }
        if (SWITCH0 == OFF && SWITCH1 == ON) {
             patternTwo();
        }
    }
}



/*******************************************************************
List of Functions
 ********************************************************************/

/*******************************************************************************
 * Function: void Initialize(void)
 * PreCondition: None
 * Input: None
 * Output: None
 * Side Effects: None
 * Overview: Initializes the microcontroller, the peripherals
 * used in the application and any global variables
 * used by multiple functions.
 * Note: None
 ******************************************************************************/
void Initialize(void) {
    CMCON0 = 7; // Turn off Comparators
    ANSEL = 0; // Make all PORTS Digital
    TRISA = 0b010001; // Setup RA0,RA4 as inputs, rest are outputs
    TRISC = 0b000000; // Setup all of PORTC pins to be outputs
}

/*******************************************************************************
 * Function: void msDelay(unsigned count)
 * PreCondition: None
 * Input: None
 * Output: None
 * Side Effects: None
 * Overview: Calls the assembler macro __delay_ms(value) in a loop
 * Note: None
 ******************************************************************************/
void msDelay(unsigned count) {
    for (unsigned int i = 0; i < count; i++) {
        __delay_ms(1);
    }
}

void startingPattern(int time) { 
    turnOnLEDs();
    msDelay(time); // delay 
    turnOffLEDs();
    msDelay(time); // delay 
}

void patternOne (void) {
            for (int i = 0; i < 3; i++) {
                clockwiseCircle();
            }
            for (int i = 0; i < 3; i++) {
                counterClockwiseCircle();
            }
            for (int i = 0; i < 3; i++) {
                twoHalfCircles();
            }
            for (int i = 200; i > 0; i-=50) {
                circleOn(i);
                circleOff(i); 
            }
                
            oppositesOnCircleCCW();
            oppositesOnCircleCW();
            triangleTurnCW();
            
            for (int i = 0; i < 2; i++) {
                squareTurnCW();
                quickFlashOpposites();
            }
            
            for (int i = 50; i <= 300; i+=25) {
                startingPattern(i);
            }  
}

void patternTwo(void) {
    delayFlash();
    for (int i = 0; i < 3; i++) {
        quickFlashOpposites();
    }
    for (int i = 0; i < 2; i++) {
        oppositesOnCircleCW();
    }
    for (int i = 0; i < 2; i++) {
        oppositesOnCircleCCW();
    }
    
    circleOn(150);
    squareTurnCW();
    
    turnOnLEDs();
    triangleTurnCW();
    turnOnLEDs();

    for (int i = 0; i < 2; i++) {
        circleOff(150);
        clockwiseCircle();
        twoHalfCircles();    
    }

	for (int i = 0; i < 3; i++) {
		circleFill();
		circleEmpty();
	}

	clockwiseCircle();
	counterClockwiseCircle();


}

void turnOnLEDs(void) {
    PORTC = LED1 | LED3 | LED4 | LED7 | LED8; // turn on all LEDs
    PORTA = LED2 | LED5 | LED6;
}

void turnOffLEDs (void) {
    PORTC = 0; // turn off all LEDs
    PORTA = 0;
}

void clockwiseCircle(void) {
    
    PORTC = LED1; // LED 1 on
    msDelay(75);
    PORTC = 0; // LED 1 off
    PORTA = LED2; // LED 2 on
    msDelay(75);
    PORTA = 0; // LED 2 off
    PORTC = LED3; // LED 3 on
    msDelay(75);
    PORTC = LED4; //LED 3 off and LED 4 on
    msDelay(75);
    PORTC = 0; // LED 4 off
    PORTA = LED5; // LED 5 on
    msDelay(75);
    PORTA = LED6; // LED 5 off and LED 6 on
    msDelay(75);
    PORTA = 0; // LED 6 off
    PORTC = LED7; // LED 7 on
    msDelay(75);
    PORTC = LED8; //LED 7 off and LED 8 on
    msDelay(75);
    PORTC = LED1; // LED 8 off and LED 1 on
    msDelay(75);
 //   turnOffLEDs();
 //   msDelay(75);
}

void counterClockwiseCircle(void) {

    PORTC = LED8; // LED 8 on
    msDelay(75);
    PORTC = LED7; // LED 7 on and LED 8 off
    msDelay(75);
    PORTC = 0; // LED 7 off
    PORTA = LED6; // LED 6 on
    msDelay(75);
    PORTA = LED5; // LED 5 on and LED 6 off
    msDelay(75);
    PORTA = 0; // LED 5 off
    PORTC = LED4; //LED 4 on
    msDelay(75);
    PORTC = LED3; // LED 3 on and LED 4 off
    msDelay(75);
    PORTC = 0; // LED 3 off
    PORTA = LED2; // LED 2 on
    msDelay(75);
    PORTA = 0; // LED 2 off
    PORTC = LED1; // LED 1 on
    msDelay(75);
 //   turnOffLEDs();
 //   msDelay(75);
}

void twoHalfCircles (void) {
    
    PORTC = LED1; // LED 1 on
    msDelay(75); 
    PORTC = LED8; // LED 8 on and LED 1 off
    PORTA = LED2; // LED 2 on
    msDelay(100);
    PORTA = 0; // LED 2 off
    PORTC = LED3 | LED7; // LED 3 on, LED 7 on and LED 8 off
    msDelay(100);
    PORTA = LED6; //LED 6 on
    PORTC = LED4; //LED 4 on, LED 3 and LED 7 off
    msDelay(100);
    PORTC = 0; // LED 4 off
    PORTA = LED5; // LED 5 on and LED 6 off
    msDelay(100);
    PORTA = LED6; //LED 6 on and LED 5 off
    PORTC = LED4; //LED 4 on
    msDelay(100);
    PORTA = 0; // LED 6 off
    PORTC = LED3 | LED7; // LED 3 on, LED 7 on and LED 4 off
    msDelay(100);
    PORTC = LED8; // LED 8 on and LED 3 and LED 7 off
    PORTA = LED2; // LED 2 on
    msDelay(100);
    PORTA = 0; // LED 2 off
    PORTC = LED1; // LED 1 on and LED 8 off
    msDelay (100);
    turnOffLEDs();
    msDelay(100);

}

void circleOn (int time) {
    PORTC = LED1; // LED 1 on
    msDelay(time);
    PORTC = LED1 | LED8; // LED 1, 8 on
    PORTA = LED2; // LED 2 on
    msDelay(time);
    PORTC = LED1 | LED3 | LED7 | LED8; // LED 1, 3, 7, 8 on
    PORTA = LED2; // LED 2 on
    msDelay(time);
    PORTC = LED1 | LED3 | LED4 | LED7 | LED8; // LED 1, 3, 4, 7, 8 on
    PORTA = LED2 | LED6; // LED 2, 6 on
    msDelay(time);
    PORTC = LED1 | LED3 | LED4 | LED7 | LED8; // LED 1, 3, 4, 7, 8 on
    PORTA = LED2 | LED5 | LED6; // LED 2, 5, 6 on
    msDelay(time);
}

void circleOff (int time) {
    PORTC = LED1 | LED3 | LED4 | LED7 | LED8; // LED 1, 3, 4, 7, 8 on
    PORTA = LED2 | LED6; // LED 2, 6 on LED 5 off
    msDelay(time);
    PORTC = LED1 | LED3 | LED7 | LED8; // LED 1, 3, 7, 8 on LED 4 off
    PORTA = LED2; // LED 2 on LED 5, 6 off
    msDelay(time);
    PORTC = LED1 | LED8; // LED 1, 8 on LED 3, 4, 7 off
    PORTA = LED2; // LED 2 on LED 5, 6 off
    msDelay(time);
    PORTA = 0; // LED 2, 5, 6 off
    PORTC = LED1; // LED 1 on LED 3, 4, 7, 8 off
    msDelay(time);
    turnOffLEDs();
    msDelay(time);

}

void oppositesOnCircleCCW(void) {
    PORTC = LED1; // LED 1 on
    PORTA = LED5; //LED 5 on
    msDelay(150);
    PORTC = LED4 | LED8; //LED 4, 8 on LED 1 off
    PORTA = 0; // LED 5 off
    msDelay(150);
    PORTC = LED3 | LED7; // LED 3,7 on LED 4,8 off
    msDelay(150);
    PORTC = 0; // LED 3, 7 off
    PORTA = LED2 | LED6; // LED 2, 6 on
    msDelay(150);
    PORTC = LED1; // LED 1 on
    PORTA = LED5; // LED 5 on, LED 2,6 off
    msDelay(150);
    turnOffLEDs();
    msDelay(100);
}

void oppositesOnCircleCW(void) {
    PORTC = LED1; // LED 1 on
    PORTA = LED5; //LED 5 on
    msDelay(150);
    PORTC = 0; // LED 1 off
    PORTA = LED2 | LED6; // LED 2, 6 on LED 5 off
    msDelay(150);
    PORTC = LED3 | LED7; // LED 3,7 on 
    PORTA = 0; // LED 2, 6 off
    msDelay(150);
    PORTC = LED4 | LED8; //LED 4, 8 on LED 3,7 off
    msDelay(150);
    PORTC = LED1; // LED 1 on LED 4,8 off
    PORTA = LED5; //LED 5 on    
    msDelay(150);
    turnOffLEDs();
    msDelay(100);
}

void triangleTurnCW(void) {
    PORTC = LED1 | LED4; //  LED 1, 4 on
    PORTA = LED6; // LED 6 on
    msDelay(300);
    PORTC = LED7; //LED 7 on, LED 1,4 off
    PORTA = LED2 | LED5; // LED 2,5 on LED 6 off
    msDelay(300);
    PORTC = LED3 | LED8; // LED 3,8 on LED 7 off
    PORTA = LED6; //LED 6 on LED 2,5 off
    msDelay(300);
    PORTC = LED4 | LED7 | LED1; // LED 4,7,1 on LED 3,8 off
    PORTA = 0; // LED 6 off
    msDelay(300);
    PORTC = LED8; // LED 8 on LED 4,7,1 off
    PORTA = LED5 | LED2; // LED 5,2 on
    msDelay(300);
    PORTC = LED1 | LED3; // LED 1,3 on LED 8 off
    PORTA = LED6; // LED 6 on LED 5,2 off
    msDelay(300);
    PORTC = LED7 | LED4; //LED 7,4 on LED 1,3 off
    PORTA = LED2; //LED 2 on LED 6 off
    msDelay(300);
    PORTC = LED8 | LED3; //LED 8,3 on LED 7,4 off
    PORTA = LED5; //LED 5 on LED 2 off
    msDelay(300);
    turnOffLEDs(); // turns off all LEDs
    msDelay(100);
}

void squareTurnCW (void) {
    PORTC = LED8 | LED4; // LED 8,4 on
    PORTA = LED2 | LED6; // LED 2,6 on
    msDelay(200);
    PORTC = LED1 | LED3 | LED7; // LED 1,3,7 on LED 8,4 off
    PORTA = LED5; // LED 5 on LED 2,6 off
    msDelay(200);
    PORTC = LED4 | LED8; // LED 4,8 on LED 1,3,7 off
    PORTA = LED2 | LED6; //LED 2,6 on LED 5 off
    msDelay(200);
    PORTC = LED3 | LED7 | LED1; // LED 3,7,1 on LED 4,8 off
    PORTA = LED5; // LED 5 on LED 2,6 off
    msDelay(200);
    PORTC = LED4 | LED8; // LED 4,8 on LED 1,3,7 off
    PORTA = LED2 | LED6; //LED 2,6 on LED 5 off
    msDelay(200);
    PORTC = LED3 | LED7 | LED1; // LED 3,7,1 on LED 4,8 off
    PORTA = LED5; // LED 5 on LED 2,6 off
    msDelay(200);
    PORTC = LED4 | LED8; // LED 4,8 on LED 1,3,7 off
    PORTA = LED2 | LED6; //LED 2,6 on LED 5 off
    msDelay(200);
    PORTC = LED3 | LED7 | LED1; // LED 3,7,1 on LED 4,8 off
    PORTA = LED5; // LED 5 on LED 2,6 off
    msDelay(200);
    turnOffLEDs(); // turns off all LEDs
    msDelay(100);
}

void quickFlashOpposites (void) {

    for (int i = 0; i < 3; i++) {
    	PORTC = LED1; // LED 1 on
        PORTA = LED5; // LED 5 on
    	msDelay(50);
        turnOffLEDs();
        msDelay(50);
    }

    for (int i = 0; i < 3; i++) {
    	PORTC = LED3 | LED7; // LED 3,7 on LED 1 off
    	PORTA = 0; // LED 5 off
    	msDelay(50);
        turnOffLEDs();
        msDelay(50);
    }

    for (int i = 0; i < 3; i++) {
    	PORTC = LED4 | LED8; // LED 4,8 on LED 3,7 off
    	msDelay(50);
        turnOffLEDs();
        msDelay(50);
    }

    for (int i = 0; i < 3; i++) {
    	PORTC = 0; // LED 4,8 off
    	PORTA = LED2 | LED6; // LED 2, 6 on
    	msDelay(50);
        turnOffLEDs();
        msDelay(50);
    }
       
}

void delayFlash(void) {
    for (int i = 300; i >= 50; i-=50) {
        turnOnLEDs();
        msDelay(500);
        turnOffLEDs();
        msDelay(i);
    }
}

void circleFill(void) {
	PORTC = LED1; // LED 1 on
	msDelay(100);
	PORTC = LED1; // LED 1 on
	PORTA = LED2; // LED 2 on
	msDelay(100);
	PORTC = LED1 | LED3; // LED 1,3 on
	PORTA = LED2; // LED 2 on
	msDelay(100);
	PORTC = LED1 | LED3 | LED4; // LED 1,3,4 on
	PORTA = LED2; // LED 2 on
	msDelay(100);
	PORTC = LED1 | LED3 | LED4; // LED 1,3,4 on
	PORTA = LED2 | LED5; // LED 2,5 on
	msDelay(100);
	PORTC = LED1 | LED3 | LED4; // LED 1,3,4 on
	PORTA = LED2 | LED5 | LED6; // LED 2,5,6 on
	msDelay(100);
	PORTC = LED1 | LED3 | LED4 | LED7; // LED 1,3,4,7 on
	PORTA = LED2 | LED5 | LED6; // LED 2,5,6 on
	msDelay(100);
	PORTC = LED1 | LED3 | LED4 | LED7 | LED8; // LED 1,3,4,7,8 on
	PORTA = LED2 | LED5 | LED6; // LED 2,5,6 on
	msDelay(100);
}

void circleEmpty(void) {
	PORTC = LED3 | LED4 | LED7 | LED8; // LED 3,4,7,8 on
	PORTA = LED2 | LED5 | LED6; // LED 2,5,6 on
	msDelay(100);	
	PORTC = LED3 | LED4 | LED7 | LED8; // LED 3,4,7,8 on
	PORTA = LED5 | LED6; // LED 5,6 on
	msDelay(100);
	PORTC = LED4 | LED7 | LED8; // LED 4,7,8 on
	PORTA = LED5 | LED6; // LED 5,6 on
	msDelay(100);
	PORTC = LED7 | LED8; // LED 7,8 on
	PORTA = LED5 | LED6; // LED 5,6 on
	msDelay(100);
	PORTC = LED7 | LED8; // LED 7,8 on
	PORTA = LED6; // LED 6 on
	msDelay(100);
	PORTC = LED7 | LED8; // LED 7,8 on
	PORTA = 0; // LED 6 off
	msDelay(100);
	PORTC = LED8; // LED 8 on
	msDelay(100);
	PORTC = 0; // LED 8 off
	msDelay(100);
}
