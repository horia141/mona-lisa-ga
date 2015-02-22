#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "utils.h"

typedef struct _rectangle  rectangle;

struct _rectangle
{
  float  x;
  float  y;
  float  w;
  float  h;
};

rectangle  rectangle_make_xywh(float x, float y, float w, float h);
rectangle  rectangle_make_tlbr(float tl_x, float tl_y, float br_x, float br_y);
rectangle  rectangle_make_copy(const rectangle* r);
void       rectangle_free(rectangle* rectangle);

bool       rectangle_is_valid(const rectangle* r);

rectangle* rectangle_overwrite(rectangle* dst, const rectangle* src);

#endif
