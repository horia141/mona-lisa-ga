#include <stdlib.h>
#include <assert.h>

#include "common.h"

bool
color_is_valid(
  const color* c)
{
  if (c == NULL) {
    return false;
  }

  if (c->r < 0 || c->r > 1) {
    return false;
  }

  if (c->g < 0 || c->g > 1) {
    return false;
  }

  if (c->b < 0 || c->g > 1) {
    return false;
  }

  if (c->a < 0 || c->a > 1) {
    return false;
  }

  return true;
}


bool
geometry_is_valid(
  const geometry* g)
{
  if (g == NULL) {
    return false;
  }

  if (g->x < 0 || g->x > 1) {
    return false;
  }

  if (g->y < 0 || g->y > 1) {
    return false;
  }

  if (g->w < 0 || g->w > 1) {
    return false;
  }

  if (g->h < 0 || g->h > 1) {
    return false;
  }

  if (g->x + g->w < 0 || g->x + g->w > 1) {
    return false;
  }

  if (g->y + g->h < 0 || g->y + g->h > 1) {
    return false;
  }

  return true;
}


bool
rectangle_is_valid(
  const rectangle* r)
{
  if (r == NULL) {
    return false;
  }

  if (!geometry_is_valid(&r->geometry)) {
    return false;
  }

  if (!color_is_valid(&r->color)) {
    return false;
  }

  return true;
}


float
max(
  float a,
  float b)
{
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

float
min(
  float a,
  float b)
{
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

int
clamp(
  int x,
  int min,
  int max)
{
  if (x < min) {
    return min;
  } else if (x > max) {
    return max;
  } else {
    return x;
  }
}

int
unirandom_i(
  int beg,
  int end)
{
  assert(beg < end);

  return beg + (random() % (end - beg));
}


float
unirandom_f(
  float beg,
  float end)
{
  assert(beg < end);

  return beg + (end - beg) * (random() / (float)RAND_MAX);
}

bool
unirandom_b()
{
  int  tmp;

  tmp = random() % 2;

  if (tmp == 0) {
    return true;
  } else {
    return false;
  }
}
