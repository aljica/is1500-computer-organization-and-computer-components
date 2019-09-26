/*
 prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/

#include <stdio.h>
#include <math.h>

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

int main(void){
    printf("%d\n", is_prime(11));  // 11 is a prime.      Should print 1.
    printf("%d\n", is_prime(383)); // 383 is a prime.     Should print 1.
    printf("%d\n", is_prime(987)); // 987 is not a prime. Should print 0.
    printf("%d\n", is_prime(8742));
}
