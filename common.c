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

  if (g->r < 0 || g->r > 1) {
    return false;
  }

  if (g->c < 0 || g->c > 1) {
    return false;
  }

  if (g->x + g->r < 0 || g->x + g->r > 1) {
    return false;
  }

  if (g->y + g->c < 0 || g->y + g->c > 1) {
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
