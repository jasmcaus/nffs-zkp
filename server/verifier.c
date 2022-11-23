#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include "zkp.h"

/*
    Sends a random bit vector as the challenge (step 2 of the protocol)
*/
unsigned int challenge() {
    int index, bit;
    unsigned int bitmask = 0x0;

    setrndseed();
    srandom((unsigned int) mpz_get_ui(rndseed));

    // printf("Challenge     : ");
    for(index = 0; index < K; index++) {
        bit = (int) (random() % 2);
        if(bit) bitmask |= (0x1 << index);
        /* the challenge is printed starting from LSB */
        // printf("%d", bit);
    }
    // printf("\n\n");
    
    return (bitmask);
}    



/*
    Verifies the response from Prover (step 4 of the protocol)
*/
int verify(mpz_t x, mpz_t y, int e) {
    int index, result = FALSE;
    mpz_t test;

    mpz_init(test);

    mpz_mul(test, y, y);

    // printf("Server: Verification (K = %i) steps starting at: ", K);
    // timestamp(stdout);
    // printf("\n");
    for(index = 0; index < K; index++) {
        // gmp_printf("[%i]: %Zd\n", index, i[index]);
        if(e & (0x1 << index)) {
            mpz_mul(test, test, i[index]);
        }
    }
    // printf("Server: Verification ended at: ");
    // timestamp(stdout);
    // printf("\n");

    mpz_mod(test, test, n);
    // gmp_printf("x = %Zd\n", x);
    // gmp_printf("test = %Zd\n", test);
    // gmp_printf("n = %Zd\n", n);
    if(mpz_cmp(x, test) == 0) 
        result = TRUE;
    mpz_clear(test);

    return (result);
}
