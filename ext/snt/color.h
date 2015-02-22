#ifndef _COLOR_H
#define _COLOR_H

#include "utils.h"

typedef struct _color  color;

struct _color
{
  float  r;
  float  g;
  float  b;
  float  a;
};

color   color_make_rgb(float r, float g, float b);
color   color_make_rgba(float r, float g, float b, float a);
color   color_make_copy(const color* c);
void    color_free(color* c);

bool    color_is_valid(const color* c);

bool    color_equal(const color* c1, const color* c2);

color*  color_overwrite(color* dst, const color* src);
float   color_distance(const color* a, const color* b);

#endif
