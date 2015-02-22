#ifndef _UTILS_H
#define _UTILS_H

#include <stdbool.h>
#include <assert.h>

#define EPS 1e-5

int    max_n(int a, int b);
int    min_n(int a, int b);
float  max_f(float a, float b);
float  min_f(float a, float b);
int    clamp_i(int x, int min, int max);
float  clamp_f(float x, float min, float max);
int    unirandom_i(int beg, int end);
float  unirandom_f(float beg, float end);
bool   unirandom_b();
bool   eps_eql(float a, float b);

#endif
