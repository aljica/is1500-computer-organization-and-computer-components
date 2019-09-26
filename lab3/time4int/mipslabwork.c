/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int prime = 1234567;

int timeoutcount = 0;

int mytime = 0x5957;

volatile int* portE = (volatile int*) 0xbf886110;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  //IFS(0) = 0; // Acknowledging interrupts from TIMER 2? [Resetting ALL event flags].

  if (IFS(0) & 0x100) {

    IFSCLR(0) = 0x100; // Clears event flag.
    timeoutcount += 1; // Incrementing our global counter.

    if (timeoutcount == 10) {
      time2string(textstring, mytime);
      display_string(3, textstring);
      display_update();
      tick(&mytime);
      timeoutcount = 0;
    }
  }

}

/* Lab-specific initialization goes here */
void labinit( void )
{
  volatile int* toggleE = (volatile int*) 0xbf886100; // address of register TRISE
  *toggleE &= ~0xff; // set bits 0 through 7 to 0 (output). This is saying that the port (port E) is an output (bit value 0 in first 8 bits of TRISE)

  //volatile int* portE = (volatile int*) 0xbf886110;
  //*portE = 0;

  TRISD = TRISD & 0xfe0; // Set bits 11 through 5 to 1 (input)

  T2CON = 0x70; // Use prescaler 1:256. If we use 1:64 then we get overflow.
  PR2 = ((80000000/256) / 10); // 10 timeouts per second. Danger: overflow!
  TMR2 = 0; // Reset the timer.
  T2CONSET = 0x8000; // Enable module. Internal peripheral clock is used (bit 1 = 0).

  IPC(2) = IPC(2) | 0x10;
  IEC(0) = 0x100;

  enable_interrupt(); // Enables the interrupt that we now have in labwork.S
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  prime = nextprime(prime);
  display_string(0, itoaconv(prime));
  display_update();
}
