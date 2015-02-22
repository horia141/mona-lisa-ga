#ifndef _IMAGE_H
#define _IMAGE_H

#include "utils.h"
#include "color.h"

typedef struct _image image;

image*        image_make_blank(int rows, int cols, const color* c);
image*        image_make_copy(const image* src);
void          image_free(image* img);

bool          image_is_valid(const image* img);

bool          image_equal(const image* img1, const image* img2, float epsilon);

image*        image_overwrite(image* dst, const image* src);
float*        image_make_texture(const image* img);
float*        image_make_texture_a(const image* img, float* texture);

int           image_get_rows(const image* img);
int           image_get_cols(const image* img);
const color*  image_get_pixels(const image* img);
color*        image_get_pixels_a(image* img);
const color*  image_get(const image* img, int row, int col);
void          image_set(image* img, int row, int col, const color* c);

image*        image_from_ppm_t(const char* ppm_path);
void          image_print_t(const image* img);

#endif
