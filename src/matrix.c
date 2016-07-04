#include <stdlib.h>
#include <errno.h>

#include "matrix.h"

matrix_t *
minit(int rownum, int colnum)
{
    matrix_t *matrix = (matrix_t *)malloc(sizeof(*matrix));
    if(matrix == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    matrix->rownum = rownum;
    matrix->colnum = colnum;
    matrix->data = (int *)malloc(rownum * colnum * sizeof(int));
    if(matrix->data == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    return matrix;
}

void
mfree(matrix_t *matrix)
{
    free(matrix->data);
    free(matrix);
}

int
mget(const matrix_t matrix, int rowi, int colj)
{
    return *(matrix.data + ((rowi * matrix.colnum) + colj));
}

void
mset(matrix_t *matrix, int rowi, int coli, int value)
{
    *(matrix->data + ((rowi * matrix->colnum) + coli)) = value;
}

matrixf_t *
mfinit(int rownum, int colnum)
{
    matrixf_t *matrix = (matrixf_t *)malloc(sizeof(*matrix));
    if(matrix == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    matrix->rownum = rownum;
    matrix->colnum = colnum;
    matrix->data = (float *)malloc(rownum * colnum * sizeof(float));
    if(matrix->data == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    return matrix;
}

void
mffree(matrixf_t *matrix)
{
    free(matrix->data);
    free(matrix);
}

float
mfget(const matrixf_t matrix, int rowi, int colj)
{
    return *(matrix.data + ((rowi * matrix.colnum) + colj));
}

void
mfset(matrixf_t *matrix, int rowi, int colj, float value)
{
    *(matrix->data + ((rowi * matrix->colnum) + colj)) = value;
}

