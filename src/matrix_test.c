#include <stdio.h>
#include <assert.h>

#include "matrix.h"

int
main(int argc, char **argv)
{
    matrix_t *matrix = minit(2, 3);
    mset(matrix, 0, 0, 10);
    assert(mget(*matrix, 0, 0) == 10);
    mfree(matrix);

    matrixf_t *matrixf = mfinit(2, 3);
    mfset(matrixf, 0, 0, 0.19);
    assert(mfget(*matrixf, 0, 0) == 0.19f);
    mffree(matrixf);

    printf("PASS TEST\n");
}
