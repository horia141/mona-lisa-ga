#include <stdlib.h>
#include <assert.h>

#include "common.h"

color*
color_copy(
  color* dst,
  const color* src)
{
  assert(color_is_valid(dst));
  assert(color_is_valid(src));

  dst->r = src->r;
  dst->g = src->g;
  dst->b = src->b;
  dst->a = src->a;

  return dst;
}

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

float
color_distance(
  const color* a,
  const color* b)
{
  assert(color_is_valid(a));
  assert(color_is_valid(b));

  return (b->r - a->r) * (b->r - a->r) +
         (b->g - a->g) * (b->g - a->g) +
         (b->b - a->b) * (b->b - a->b) +
         (b->a - a->a) * (b->a - a->a);
}


geometry*
geometry_copy(
  geometry* dst,
  const geometry* src)
{
  assert(geometry_is_valid(dst));
  assert(geometry_is_valid(src));

  dst->x = src->x;
  dst->y = src->y;
  dst->w = src->w;
  dst->h = src->h;

  return dst;
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

  /* if (g->x + g->w < 0 || g->x + g->w > 1) { */
  /*   return false; */
  /* } */

  /* if (g->y + g->h < 0 || g->y + g->h > 1) { */
  /*   return false; */
  /* } */

  return true;
}


rectangle*
rectangle_copy(
  rectangle* dst,
  const rectangle* src)
{
  assert(rectangle_is_valid(dst));
  assert(rectangle_is_valid(src));

  geometry_copy(&dst->geometry,&src->geometry);
  color_copy(&dst->color,&src->color);

  return dst;
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
clamp_i(
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

float
clamp_f(
  float x,
  float min,
  float max)
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
