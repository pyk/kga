#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include "vector.h"

/* Integer vector */
vector_t *
vinit(const size_t n)
{
    vector_t *v = malloc(sizeof *v);
    if(v == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    int *data = malloc(n * sizeof *data);
    if(data == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    v->data = data;
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

int
vresize(vector_t *const v, const size_t size)
{
    int *new_data = realloc(v->data, size * sizeof *new_data);
    if(new_data == NULL) {
        errno = ENOMEM;
        return -1;
    }
    v->data = new_data;
    v->totaln = size;
    return v->totaln;
}

int
vpush(vector_t *v, const int value)
{
    if(v->currentn < v->totaln) {
        *(v->data + v->currentn) = value;
        v->currentn += 1;
    } else {
        int ret = vresize(v, v->totaln + 1);
        if(ret < 0) {
            errno = ENOMEM;
            return -1;
        }
        *(v->data + v->currentn) = value;
        v->currentn += 1;
    }
    return v->currentn;
}

void
vpop(vector_t *const v)
{
    if(v->currentn > 0) {
        *(v->data + v->currentn) = 0;
        v->currentn -= 1;
    }
}

int
vget(const vector_t v, const int index)
{
    if(index < 0 || index >= v.currentn) return -1;
    int value = *(v.data + index);
    return value;
}

/* Float vector */
vectorf_t *
vfinit(const size_t n)
{
    vectorf_t *v = malloc(sizeof *v);
    if(v == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    float *data = malloc(n * sizeof *data);
    if(data == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    v->data = data;
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

int
vfresize(vectorf_t *const v, size_t size)
{
    float *new_data = realloc(v->data, size * sizeof *new_data);
    if(new_data == NULL) {
        errno = ENOMEM;
        return -1;
    }
    v->data = new_data;
    v->totaln = size;
    return v->totaln;
}

int
vfpush(vectorf_t *const v, const float value)
{
    if(v->currentn < v->totaln) {
        *(v->data + v->currentn) = value;
        v->currentn += 1;
    } else {
        int ret = vfresize(v, v->totaln + 1);
        if(ret < 0) {
            errno = ENOMEM;
            return -1;
        }
        *(v->data + v->currentn) = value;
        v->currentn += 1;
    }
    return v->currentn;
}

void
vfpop(vectorf_t *const v)
{
    if(v->currentn > 0) {
        *(v->data + v->currentn) = 0;
        v->currentn -= 1;
    }
}

float
vfget(const vectorf_t v, const int index)
{
    if(index < 0 || index >= v.currentn) return -1;
    float value = *(v.data + index);
    return value;
}

