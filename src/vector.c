#include <stdlib.h>
#include <errno.h>

#include "vector.h"

/* Integer vector */
vector_t *
vinit(const int n)
{
    vector_t *v = (vector_t *)malloc(sizeof(*v));
    if(v == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    v->data = (int *)malloc(n * sizeof(int));
    if(v->data == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    v->totaln = n;
    v->currentn = 0;
    return v;
}

void
vdestroy(vector_t *v)
{
    free(v->data);
    free(v);
}

vector_t *
vresize(vector_t *v, size_t size)
{
    v->data = (int *)realloc(v->data, size * sizeof(int));
    if(v == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    v->totaln = size;
    return v;
}

int
vpush(vector_t *v, const int value)
{
    if(v->currentn < v->totaln) {
        *(v->data + v->currentn) = value;
        v->currentn += 1;
    } else {
        v = vresize(v, v->totaln + 1);
        if(v == NULL) {
            errno = ENOMEM;
            return -1;
        }
        *(v->data + v->currentn) = value;
        v->currentn += 1;
    }
    return v->currentn;
}

void
vpop(vector_t *v)
{
    if(v->currentn > 0) {
        *(v->data + v->currentn) = 0;
        v->currentn -= 1;
    }
}

int
vget(const vector_t *v, const int index)
{
    if(index < 0 || index >= v->currentn) return -1;
    int value = *(v->data + index);
    return value;
}

/* Float vector */
vectorf_t *
vfinit(const int n)
{
    vectorf_t *v = (vectorf_t *)malloc(sizeof(*v));
    if(v == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    v->data = (float *)malloc(n * sizeof(float));
    if(v->data == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    v->totaln = n;
    v->currentn = 0;
    return v;
}

void
vfdestroy(vectorf_t *v)
{
    free(v->data);
    free(v);
}

vectorf_t *
vfresize(vectorf_t *v, size_t size)
{
    v->data = (float *)realloc(v->data, size * sizeof(float));
    if(v == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    v->totaln = size;
    return v;
}

int
vfpush(vectorf_t *v, const float value)
{
    if(v->currentn < v->totaln) {
        *(v->data + v->currentn) = value;
        v->currentn += 1;
    } else {
        v = vfresize(v, v->totaln + 1);
        if(v == NULL) {
            errno = ENOMEM;
            return -1;
        }
        *(v->data + v->currentn) = value;
        v->currentn += 1;
    }
    return v->currentn;
}

void
vfpop(vectorf_t *v)
{
    if(v->currentn > 0) {
        *(v->data + v->currentn) = 0;
        v->currentn -= 1;
    }
}

float
vfget(const vectorf_t *v, const int index)
{
    if(index < 0 || index >= v->currentn) return -1;
    float value = *(v->data + index);
    return value;
}

