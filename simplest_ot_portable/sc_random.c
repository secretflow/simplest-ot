#include "sc.h"
#include "randombytes.h"

void sc_random(unsigned char *x, int c) {

    randombytes(x, 32);

    if (c == 0)
    {
        x[31] &= 15;
    }
    else
    {
        x[0] &= 248;
        x[31] &= 127;
    }

}
