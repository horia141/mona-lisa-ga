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

color*      color_copy(color* dst, const color* src);
bool        color_is_valid(const color* c);
float       color_distance(const color* a, const color* b);

struct _geometry
{
  float  x;
  float  y;
  float  w;
  float  h;
};

geometry*    geometry_copy(geometry* dst, const geometry* src);
bool         geometry_is_valid(const geometry* g);

struct _rectangle
{
  geometry  geometry;
  color     color;
};

rectangle*  rectangle_copy(rectangle* dst, const rectangle* src);
bool        rectangle_is_valid(const rectangle* r);


float       max(float a, float b);
float       min(float a, float b);
int         clamp(int x, int min, int max);
int         unirandom_i(int beg, int end);
float       unirandom_f(float beg, float end);
bool        unirandom_b();

#endif
