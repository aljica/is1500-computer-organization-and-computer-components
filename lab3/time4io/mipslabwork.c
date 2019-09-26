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

  //volatile int* toggleD = (volatile int*) TRISD;
  //*toggleD |= 0xfe0; // set bits 11 thru 5 to 1 (input)

  TRISD = TRISD & 0xfe0; // Set bits 11 through 5 to 1 (input)
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  volatile int* portE = (volatile int*) 0xbf886110;
  delay( 1000 );

  int btnsValue = getbtns();
  int switchesValue = getsw();

  if (btnsValue == 4 || btnsValue == 5 || btnsValue == 6 || btnsValue == 7) {
    // BTN4 is pressed down.
    mytime &= 0x0fff;
    mytime = (switchesValue << 12) | mytime;
  }

  if (btnsValue == 2 || btnsValue == 3 || btnsValue == 6 || btnsValue == 7) {
    // BTN3 is pressed down.
    mytime &= 0xf0ff;
    mytime = (switchesValue << 8) | mytime;
  }

  if (btnsValue == 1 || btnsValue == 3 || btnsValue == 5 || btnsValue == 7) {
    // BTN2 is pressed down.
    mytime &= 0xff0f;
    mytime = (switchesValue << 4) | mytime;
  }

  *portE += 1;
  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
  display_image(96, icon);
}
