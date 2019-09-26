#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define COLUMNS 6
int numCalls = 0;

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

void print_sieves(int n) {
  bool numbers[n]; // local array declaration for storing integers 2 to n
  for(int i=0; i<n; i++) {
    numbers[i] = true; // set all to true
  }

  for(int j=2; j<sqrt(n); j++) {
    if (numbers[j] == true) {
      for(int k=j*j; k<n; k=k+j) {
        if (numbers[k] == true) {
          numbers[k] = false;
        }
      }
    }
  }

  for(int q=2; q<(n); q++) {
    if (numbers[q] == true) {
     print_number(q);
   }
  }

  printf("\n");

}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
  if(argc == 2)
    print_sieves(atoi(argv[1]));
  else
    printf("Please state an interger number.\n");
  return 0;
}
