/*
 print-prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define COLUMNS 6
int numCalls = 0; // to be used in print_number to keep track of columns

int is_prime(int n){
    int divisor = 2;
    int i = 1;
    double rootOfN;
    rootOfN = sqrt(n);

    if(n == 0 || n == 1) {
        return 0; // automatically not prime
    }

    while(i != 0) {
        if(divisor > rootOfN) {
            i = 0;
            return 1; // is prime
        }

        if(n % divisor == 0) {
            i = 0;
            return 0; // not prime
        }
        else if(n % divisor != 0) {
            divisor += 1; // divisor should actually always be a prime, but... screw it.
            // lab wants it simple, so they're gonna get simple. =)
        }
    }
}

void print_number(int n){

  //int *b;
  //b = &numCalls; // address of var numCalls is assigned to b

  if(numCalls == COLUMNS) {
    numCalls = 1;
    printf("\n"); // six columns in previous row already exist, so start on next row.
    printf("%10d ", n);
  } else {
    printf("%10d ", n);
    ++numCalls;
    //++*b; // increment numCalls
  }
}

void print_primes(int n){
  // Should print out all prime numbers less than 'n'
  // with the following formatting. Note that
  // the number of columns is stated in the define
  // COLUMNS

  for (int i=2; i<=n; i++) {
    int a = is_prime(i);

    if (a) {
      print_number(i);
    } else {
      continue;
    }
  }

  printf("\n");
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
  if(argc == 2)
    print_primes(atoi(argv[1]));
  else
    printf("Please state an interger number.\n");
  return 0;
}
