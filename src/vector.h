#ifndef VECTOR_H
#define VECTOR_H

struct vector {
    int *data;
    int totaln;
    int currentn;
};
typedef struct vector vector_t;

vector_t *vinit(const size_t n);
void     vpop(vector_t *const v);
void     vdestroy(vector_t *v);
int      vget(const vector_t v, const int index);
int      vresize(vector_t *const v, const size_t size);
int      vpush(vector_t *const v, const int value);

struct vectorf {
    float *data;
    int totaln;
    int currentn;
};
typedef struct vectorf vectorf_t;

vectorf_t *vfinit(const size_t n);
void      vfpop(vectorf_t *const v);
void      vfdestroy(vectorf_t *v);
float     vfget(const vectorf_t v, const int index);
int       vfresize(vectorf_t *const v, const size_t size);
int       vfpush(vectorf_t *const v, const float value);

#endif

