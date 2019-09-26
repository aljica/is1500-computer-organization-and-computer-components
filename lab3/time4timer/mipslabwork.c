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

int timeoutcount = 0;

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  volatile int* toggleE = (volatile int*) 0xbf886100; // address of register TRISE
  *toggleE &= ~0xff; // set bits 0 through 7 to 0 (output). This is saying that the port (port E) is an output (bit value 0 in first 8 bits of TRISE)

  TRISD = TRISD & 0xfe0; // Set bits 11 through 5 to 1 (input)

  T2CON = 0x70; // Use prescaler 1:256. If we use 1:64 then we get overflow.
  PR2 = ((80000000/256) / 10); // 10 timeouts per second. Danger: overflow!
  //T2CONSET = 0x70; // Set the prescaler.
  TMR2 = 0; // Reset the timer.
  T2CONSET = 0x8000; // Enable module. Internal peripheral clock is used (bit 1 = 0).
  //IFS(0) = 0; // Reset all event flags to begin with. Is this necessary?
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  volatile int* portE = (volatile int*) 0xbf886110;

  int btnsValue = getbtns();
  int switchesValue = getsw();

  if (btnsValue == 4 || btnsValue == 5 || btnsValue == 6 || btnsValue == 7) {
    // BTN4 is pressed down.
    mytime &= 0x0fff;
    mytime += (switchesValue << 12);
  }

  if (btnsValue == 2 || btnsValue == 3 || btnsValue == 6 || btnsValue == 7) {
    // BTN3 is pressed down.
    mytime &= 0xf0ff;
    mytime += (switchesValue << 8);
  }

  if (btnsValue == 1 || btnsValue == 3 || btnsValue == 5 || btnsValue == 7) {
    // BTN2 is pressed down.
    mytime &= 0xff0f;
    mytime += (switchesValue << 4);
  }

  if (IFS(0) & 0x100) {

    timeoutcount += 1; // Increment our global counter.
    IFS(0) = 0; // Reset all event flags. Not good to do this because it resets event flags for all other devices, too.
    // Can use IFSCLR(0) = 0x100 instead?

    if (timeoutcount == 10) {
      time2string( textstring, mytime );
      display_string( 3, textstring );
      display_update();
      tick( &mytime );
      display_image(96, icon);
      timeoutcount = 0;
    }
  }
}
