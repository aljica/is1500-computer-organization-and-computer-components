#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define COLUMNS 6
int numCalls = 0;

void print_number(int n){

  if(numCalls == COLUMNS) {
    numCalls = 1;
    printf("\n"); // six columns in previous row already exist, so start on next row.
    printf("%10d ", n);
  } else {
    printf("%10d ", n);
    ++numCalls;
  }
}

void print_sieves(int n) {
  bool *numbers = (bool*)malloc(n * sizeof(bool));

  for(int i=0; i<n; i++) {
    numbers[i] = true; // set all to true
  }

  time_t start, end;
  double elapsed;
  start = time(NULL);

  for(int j=2; j<sqrt(n); j++) {
    end = time(NULL);
    elapsed = difftime(end, start);
    if (elapsed>=2){
      int m = j*j; // j < sqrt(n), so let's take the j squared (to see how far we've gotten)
      int finding = 1;
      while (finding) {
        if (numbers[m] == false) {
          m = m - 1; // check one before, until true
          continue;
        }
        else {
          printf("%d\n", m); // this is our highest prime we've found in 2 seconds
          finding = 0;
          exit(0);
        }
      }
    }
    else {
      if (numbers[j] == true) {
        for(int k=j*j; k<n; k=k+j) {
          if (numbers[k] == true) {
            numbers[k] = false;
          }
        }
      }
    }
  }
/*
  for(int q=2; q<n; q++) {
    if (numbers[q] == true) {
      print_number(q);
   }
 }*/

  free(numbers);

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
