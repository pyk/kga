#include <stdio.h>
#include <assert.h>

#include "vector.h"

int
main(int argc, char **argv)
{
    vector_t *vector = vinit(1);

    int value = 10;
    vpush(vector, value);
    int result = vget(vector, 0);
    assert(value == result);
    assert(vector->currentn == 1);
    assert(vector->totaln == 1);

    value = 13;
    vpush(vector, value);
    result = vget(vector, 1);
    assert(value == result);
    assert(vector->currentn == 2);
    assert(vector->totaln == 2);

    vdestroy(vector);

    vectorf_t *vectorf = vfinit(1);

    float valuef = 10.0;
    vfpush(vectorf, valuef);
    float resultf = vfget(vectorf, 0);
    assert(valuef == resultf);
    assert(vectorf->currentn == 1);
    assert(vectorf->totaln == 1);

    valuef = 0.13;
    vfpush(vectorf, valuef);
    resultf = vfget(vectorf, 1);
    assert(valuef == resultf);
    assert(vectorf->currentn == 2);
    assert(vectorf->totaln == 2);

    vfdestroy(vectorf);

    printf("PASS TEST\n");
}
