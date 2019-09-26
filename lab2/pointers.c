#include <stdio.h>
<<<<<<< HEAD
#include <stdlib.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";
int list1[20]; // length of the list (20 words/ 20 ints)
int list2[20]; // length of the list (20 words/ 20 ints)int list2 [80]; // length of the list (20 words/ 20 ints)
int count = 0;  // counter for number of chars worked with

void copycodes (int* list, char* text, int* count){
  *list = *text;
  while(*text != 0){
    list++;
    text++;
    *count = *count + 1;
    *list = *text;
  }
}

void work (){
    copycodes(list1, text1, &count);
    copycodes(list2, text2, &count);
=======

char *text1 = "This is a string.";
char *text2 = "Yet another thing.";

int list1[20];
int list2[20];
int count = 0;

void copycodes(char *text, int *list, int *count) {

  while (*text != 0 /* while char not \0 */) {
    *list = *text; /* let the first int element in the array list
    be equal to the value of the first char byte from text */

    text++; // increment the char array by one byte.
    // this is same as addi $a0, $a0, 1.
    list++; // this is addi $a1, $a1, 4. 4 because 4 bytes,
    // because list is an int array (ints are 4 bytes in size)

    (*count)++; // (*count) dereferences count (remember count
    // gives us the MEMORY ADDRESS of count) so that we are
  // are accessing the actual value count points to)
  /* answer to question: corresponding deferencing in
  assembler is lw $t1, 0($a2) */
  }
}

void work() {
  /* an array is just a pointer to the FIRST element
  in the array. As such, when the second parameter of copycodes()
  wants an int* (i.e. a pointer to an int), which it also gets
  if we just simply pass list1 as argument, because list1 is just
  a pointer to the first int in the array list1[20].

  On the OTHER hand, we must send in the ADDRESS OF count (i.e.
  &count) for the third and final parameter of copycodes().

  similarly as list1, char* text1 is an array of chars, each
  one byte in size. as such, we only need to pass text1 as
  parameter value. */
  copycodes(text1, list1, &count);
  copycodes(text2, list2, &count);
>>>>>>> ffc16cc09d6b5ed12a745e5f8f04929ad5009ff4
}

void printlist(const int* lst){
  printf("ASCII codes and corresponding characters.\n");
  while(*lst != 0){
    printf("0x%03X '%c' ", *lst, (char)*lst);
    lst++;
  }
  printf("\n");
}

void endian_proof(const char* c){
  printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n",
         (int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));
  /* my system is little endian because the byte 0x23 is found
  to the far left, and the byte is the least significant byte.
  also see:
  https://www.geeksforgeeks.org/little-and-big-endian-mystery/
  and
  https://serverfault.com/questions/163487/how-to-tell-if-a-linux-system-is-big-endian-or-little-endian
  */

}

int main(void){
  work();

  printf("\nlist1: ");
  printlist(list1);
  printf("\nlist2: ");
  printlist(list2);
  printf("\nCount = %d\n", count);

  endian_proof((char*) &count);
}

/* IMPORTANT about advantage of little-endian vs big-endian
In “Little Endian” form, assembly language instructions for picking up a 1, 2, 4, or longer byte number proceed in exactly the same way for all formats: first pick up the lowest order byte at offset 0. Also, because of the 1:1 relationship between address offset and byte number (offset 0 is byte 0), multiple precision math routines are correspondingly easy to write.

In “Big Endian” form, by having the high-order byte come first, you can always test whether the number is positive or negative by looking at the byte at offset zero. You don’t have to know how long the number is, nor do you have to skip over any bytes to find the byte containing the sign information. The numbers are also stored in the order in which they are printed out, so binary to decimal routines are particularly efficient.
link: https://thebittheories.com/little-endian-vs-big-endian-b4046c63e1f2
