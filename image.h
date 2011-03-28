#ifndef _IMAGE_H
#define _IMAGE_H

#include <stdbool.h>

#include "common.h"

typedef struct _image image;

image*  image_blank(int rows, int cols, color c);
void    image_free(image* img);

bool    image_is_valid(const image* img);

float*  image_make_texture(const image* img);

int     image_get_rows(const image* img);
int     image_get_cols(const image* img);
color   image_get(const image* img, int row, int col);
void    image_set(image* img, int row, int col, const color c);

image*  image_from_ppm_t(const char* ppm_path);

#endif
