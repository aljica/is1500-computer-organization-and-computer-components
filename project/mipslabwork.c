/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   This file modified 2018-12-06 by Almir Aljic & Simon Sirén

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int timeoutcount = 0; // For the interrupts.

int countdown = 8; // For start_in_three_seconds().

volatile int* portE = (volatile int*) 0xbf886110; // LED lights.

int size; // Size of array. Greater size, greater difficulty.

int error = 0; // Unnecessary flag variable. Not a vital program dependency.

/* Initially, we wanted timer_difficulty to determine the game's difficulty
by only giving the user so much time to finish the game. But instead, we
changed how difficulty works by expanding the array instead. As a result,
timer_difficulty is always set to 2550, which is until all 8 LED lights are
turned on.
*/
int timer_difficulty = 2550; // See comment above.

int moves = 0; // Number of made moves. Not used. Not a vital program dependency.

/* Character array size of 6 below means the largest array we can have
is 654321 (also given by variable reset). Very easy to change.
If you want larger arrays, simply change the 6's to whichever number you want
and change
char reset[<number>] = "<number><number-1> ... 654321";
accordingly.
Naturally, you have to add additional if-statements in intro_menu()
to change the size variable accordingly. See also comment in for-loop of
reset_game().
*/
char first[6] = "4321";
char second[6] = "0000";
char third[6] = "0000";

char reset[6] = "654321";

int selected = 0; // If a number has been selected from a row: selected = 1. Else 0.
char selected_value; // The selected value from a row.

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{

  intro_menu(); // Initialize game.

  volatile int* toggleE = (volatile int*) 0xbf886100; // Address of register TRISE.
  *toggleE &= ~0xff; // For LED lights.

  TRISD = TRISD & 0xfe0; // Set bits as inputs for the buttons.

  T2CON = 0x70; // Use prescaler 1:256. If we use 1:64 then we get overflow.
  PR2 = ((80000000/256) / 10); // 10 timeouts per second. Danger: overflow!
  TMR2 = 0; // Reset the timer.
  T2CONSET = 0x8000; // Enable module. Internal peripheral clock is used (bit 1 = 0).
}

void select_from_array (char* arr) {

  int i = 0;

  if (*arr == '0') {

    // Then, array is empty, because element at index 0 is '0'.
    return;

  } else {

    while (i<size) { // For each element in the array.

      if (*arr == '0') {

        arr--; // We want the previous element. i.e. arr[i-1].

        // Selecting that value:
        selected_value = *arr;
        *arr = '0';
        selected = 1;
        break;

      } else {

        if (i == (size-1)) { // If we have reached the end of the array.

          // Then, array is full. Select this last element.
          selected_value = *arr;
          *arr = '0';
          selected = 1;
          break;

        } else {

          // Else, keep looping.

          i++; // For each element in the array.
          arr++; // So that next iteration we will be at index arr[i+1].

        }

      }

    }

  }

}

void drop_to_array (char* arr) {

  int j = 0;

  while (j<size) { // For each element in the array.

    if (*arr == '0') {

      if (j != 0) { // Only if it's not the first element in the array.
        arr--; // Decrement index so that we get arr[j-1].
        // The reason for this is so that we can check if the value we're
        // trying to drop is smaller than the outer-most value in the array.
      }
      else {
        // Otherwise, just drop the selected value into the array,
        // because the array is empty.
        *arr = selected_value;
        selected = 0;
        moves++;
        break;
      }

      if (selected_value >= *arr) {
        // The value we're trying to drop is greater than the outer-most value
        // in the array. Thus, we cannot drop it there.
        display_string(3, "Not there!");
        display_update();
        break;
      } else {
        // Otherwise, drop it.
        arr++;
        *arr = selected_value;
        selected = 0;
        moves++;
        break;
      }

    } else { // Otherwise, next iteration.

      if (j == (size-1)) {

        // Then, array is full. This mustn't ever happen.
        break;

      } else {
        // Next iteration.
        j++;
        arr++;

      }

    }

  }

}

void start_in_three_seconds () {
  while (countdown != 0) {
    delay(1000);

    if (countdown > 4) {
      display_string(0, "If all lights");
      display_string(1, "turn on, you");
      display_string(2, "will lose!");
      display_string(3, "Be fast!");
    }

    if (countdown == 4) {
      display_string(0, "Start in 3s!");
    }

    else if (countdown == 3) {
      display_string(1, "Get ready!");
    }

    else if (countdown == 2) {
      display_string(2, "Set!");
    }

    else if (countdown == 1) {
      display_string(3, "Go!");
    }

    display_update();
    countdown -= 1;
  }

  countdown = 8;
}

void intro_menu() {
  while (1) {
    display_string(0, "Choose difficulty!");
    display_string(1, "BTN4: Easy");
    display_string(2, "BTN3: Hard");
    display_string(3, "BTN2: Expert");

    int buttons = getbtns();

    if (buttons == 4 || buttons == 2 || buttons == 1){
      if (buttons == 4) {
        size = 4; // Size of array. Difficulty level.
      } else if (buttons == 2) {
        size = 5;
      } else if (buttons == 1) {
        size = 6;
      }
      reset_game(first, second, third);
      start_in_three_seconds();
      break;
    }

    display_update();

  }
}

void reset_game(char* first, char* second, char* third) {
  selected = 0;

  int j;

  for (j=1; j<=size; j++) {
    // first[size-j] = reset[6-j].
    // For larger arrays, the 6 in reset[6-j] must be changed to desired
    // array length. See comment at top of file regarding larger arrays.
    first[size-j] = reset[6-j];
    second[size-j] = '0';
    third[size-j] = '0';
  }

  *portE = 0;
  moves = 0;
}

/* This function is called repetitively from the main program */
void labwork( void )
{

  int btns = getbtns();

  if (btns == 4) { // BTN4 is pushed.
    if (selected == 0) {
      select_from_array(first);
    } else {
      drop_to_array(first);
    }
    delay(200);
  }

  else if (btns == 2) { // BTN3.
    if (selected == 0) {
      select_from_array(second);
    } else {
      drop_to_array(second);
    }
    delay(200);
  }

  else if (btns == 1) { // BTN2.
    if (selected == 0) {
      select_from_array(third);
    } else {
      drop_to_array(third);
    }
    delay(200);
  }

  else if (btns != 1 || btns != 2 || btns != 4) {
    if (btns != 0) { // Several buttons at once.
      display_string(3, "One at a time!!");
    } else {
      display_string(3, "");
    }
  }

  // Display arrays on LCD

  display_string(0, first);
  display_string(1, second);
  display_string(2, third);

  display_update();

  // Check if user has won.
  // If second or third arrays have a 1 at the end, then logically enough
  // they have created an array like "654321", i.e. they have won the game.

  if (third[size-1] == '1' || second[size-1] == '1') {
    timeoutcount = 0;
    while (btns != 7) {
      display_string(3, "congrats!");
      display_update();
      btns = getbtns();
      if (btns == 7) {
        delay(200);
        intro_menu();
      }
    }
  }

  // Can't take too long to finish!

  if (IFS(0) & 0x100) {

    timeoutcount += 1; // Increment our global counter.
    IFS(0) = 0; // Reset all event flags. Not good to do this because it resets event flags for all other devices, too.

    if(timeoutcount % 10 == 0) { // Every ten interrupts (each second).
      *portE += 1; // Add another second.
    }

    if (timeoutcount == timer_difficulty) { // If time is up and game is lost.
      timeoutcount = 0;
      while (btns!=7) {
        display_string(3, "You failed!");
        display_update();
        btns = getbtns();

        if (btns == 7) {
          delay(200);
          intro_menu(); // Restart game. (Also game initialization).
        }
      }
    }
  }
}
