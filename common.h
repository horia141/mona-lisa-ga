#ifndef _COMMON_H
#define _COMMON_H

#include <stdbool.h>

typedef struct _color       color;
typedef struct _geometry    geometry;
typedef struct _rectangle   rectangle;

struct _color
{
  float  r;
  float  g;
  float  b;
  float  a;
};

bool  color_is_valid(const color* c);

struct _geometry
{
  float  x;
  float  y;
  float  r;
  float  c;
};

bool  geometry_is_valid(const geometry* g);

struct _rectangle
{
  geometry  geometry;
  color     color;
};

bool rectangle_is_valid(const rectangle* r);

#endif
