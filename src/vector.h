#ifndef VECTOR_H
#define VECTOR_H

struct vector {
    int *data;
    int totaln;
    int currentn;
};
typedef struct vector vector_t;

vector_t *vinit(const int n);
vector_t *vresize(vector_t *v, size_t size);
void     vpop(vector_t *v);
void     vdestroy(vector_t *v);
int      vpush(vector_t *v, const int value);
int      vget(const vector_t v, const int index);

struct vectorf {
    float *data;
    int totaln;
    int currentn;
};
typedef struct vectorf vectorf_t;

vectorf_t *vfinit(const int n);
vectorf_t *vfresize(vectorf_t *v, size_t size);
void      vfpop(vectorf_t *v);
void      vfdestroy(vectorf_t *v);
int       vfpush(vectorf_t *v, const float value);
float     vfget(const vectorf_t v, const int index);

#endif

