#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(void) {
  volatile int* getD = (volatile int*) PORTD;

  int return_value = *getD >> 8;
  return_value = return_value & 0xf;

  return return_value;
}

int getbtns(void) {
  volatile int* getDbuttons = (volatile int*) PORTD;

  int return_value = *getDbuttons >> 5;
  return_value &= 7;

  return return_value;
}
