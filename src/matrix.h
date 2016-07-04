#ifndef MATRIX
#define MATRIX

struct matrix {
    int rownum;
    int colnum;
    int *data;
};
typedef struct matrix matrix_t;

matrix_t *minit(int rownum, int colnum);
void mfree(matrix_t *matrix);
int  mget(const matrix_t matrix, int rowi, int colj);
void mset(matrix_t *matrix, int rowi, int colj, int value);

struct matrixf {
    int rownum;
    int colnum;
    float *data;
};
typedef struct matrixf matrixf_t;

matrixf_t *mfinit(int rownum, int colnum);
void  mffree(matrixf_t *matrix);
float mfget(const matrixf_t matrix, int rowi, int colj);
void  mfset(matrixf_t *matrix, int rowi, int colj, float value);

#endif
