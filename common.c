#include <assert.h>

#include "common.h"

bool
color_is_valid(
  const color c)
{
  if (c.r < 0 || c.r > 1) {
    return false;
  }

  if (c.g < 0 || c.g > 1) {
    return false;
  }

  if (c.b < 0 || c.g > 1) {
    return false;
  }

  if (c.a < 0 || c.a > 1) {
    return false;
  }

  return true;
}
