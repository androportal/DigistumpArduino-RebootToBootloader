/*
  Reboot to boot is for booting to bootloader(0x0000 position) from application program,time set id done through timer1.Timer time,and port pin for LED(useful to know which section of code is executing whether bootloader or application) are configurable.

This is especialy useful digispark attiny85.
Turns on an LED on for one second, then off for one second, repeatedly.

  
  This example code is in the public domain.

  modified 27 May 2015
Padmakar Reddy Thatikonda
 */
 
#include <Arduino.h>
#include <util/delay.h>


#include "Reboot_to_boot.h"


void initTimerCounter1(void);

int matchDelay;
static int intr_value=0;
static char led_test_flag=0,pin_no;



void initTimerCounter1(void)
{
    TCCR1 = 0;                  //stop the timer
    TCNT1 = 0;                  //zero the timer
    GTCCR = _BV(PSR1);          //reset the prescaler
    OCR1A = 244;                //set the compare value
    OCR1B = 243;
    TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A
    //start timer, ctc mode, prescaler clk/16384    
    TCCR1 = _BV(CTC1) | _BV(CS13) | _BV(CS12) | _BV(CS11) | _BV(CS10);
    sei();
}

ISR(TIMER1_COMPA_vect)
{
  if( intr_value <= matchDelay) {
  TCNT1 = 0; 
  intr_value++;
  }
  else
  {   
    cli();
    TCCR1 = 0;                  //stop the timer 
    intr_value = 0;
    if(led_test_flag==1){
       digitalWrite(pin_no, LOW);   // turn off the LED on (LOW is the voltage level)
    }
    reboot();
  }
}




// the setup function . delay is the time for reboot,pin is which pin for LED to set ,flag is for selecting/unselecting led function. 
// timer timeout for delay of 10 is 2.4 sec
void reboot_setup(int delay, char pin, char flag) {
  
  intr_value=0;
  matchDelay=delay;
  pin_no = pin;
  led_test_flag = flag; // if flag is 1 LED function selected 
  if(led_test_flag==1){
    
  // initialize digital pin_no as an output.
  pinMode(pin_no, OUTPUT);
  // For Testing 
  digitalWrite(pin_no, HIGH);   // turn the LED on (HIGH is the voltage level)
  
  }
  initTimerCounter1();  
 
}




void reboot(void) {
  
  noInterrupts(); // disable interrupts which could mess with changing prescaler
  CLKPR = 0b10000000; // enable prescaler speed change
  CLKPR = 0; // set prescaler to default (16mhz) mode required by bootloader
  void (*ptrToFunction)(); // allocate a function pointer
  ptrToFunction = 0x0000; // set function pointer to bootloader reset vector
  (*ptrToFunction)(); // jump to reset, which bounces in to bootloader
}
