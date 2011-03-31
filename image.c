#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "image.h"

struct _image
{
  int     rows;
  int     cols;
  color   data[];
};

image*
image_blank(
  int rows,
  int cols,
  color c)
{
  assert(rows > 0);
  assert(cols > 0);
  assert(color_is_valid(&c));

  image*  new_img;
  int     i;
  int     j;

  new_img = malloc(sizeof(image) + sizeof(color) * rows * cols);

  new_img->rows = rows;
  new_img->cols = cols;

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      new_img->data[i * cols + j].r = c.r;
      new_img->data[i * cols + j].g = c.g;
      new_img->data[i * cols + j].b = c.b;
      new_img->data[i * cols + j].a = c.a;
    }
  }
		   
  return new_img;
}

void
image_free(
  image* img)
{
  assert(image_is_valid(img));

  img->rows = -1;
  img->cols = -1;

  free(img);
}



bool
image_is_valid(
  const image* img)
{
  if (img == NULL) {
    return false;
  }

  if (img->rows < 1) {
    return false;
  }

  if (img->cols < 1) {
    return false;
  }

  return true;
}


float*
image_make_texture(const image* img)
{
  assert(image_is_valid(img));

  float*  new_texture;
  color   tmp;
  int     i;
  int     j;

  new_texture = malloc(sizeof(float) * img->rows * img->cols * 4);

  return image_make_texture_a(img,new_texture);
}

float*
image_make_texture_a(
  const image* img,
  float* texture)
{
  assert(image_is_valid(img));
  assert(texture != NULL);

  color   tmp;
  int     i;
  int     j;

  for (i = 0; i < img->rows; i++) {
    for (j = 0; j < img->cols; j++) {
      tmp = image_get(img,i,j);
      texture[(img->rows - i - 1) * img->cols * 4 + j * 4 + 0] = tmp.r;
      texture[(img->rows - i - 1) * img->cols * 4 + j * 4 + 1] = tmp.g;
      texture[(img->rows - i - 1) * img->cols * 4 + j * 4 + 2] = tmp.b;
      texture[(img->rows - i - 1) * img->cols * 4 + j * 4 + 3] = tmp.a;
    }
  }

  return texture;
}


int
image_get_rows(
  const image* img)
{
  assert(image_is_valid(img));

  return img->rows;
}

int
image_get_cols(
  const image* img)
{
  assert(image_is_valid(img));

  return img->cols;
}

color
image_get(
  const image* img,
  int row,
  int col)
{
  assert(image_is_valid(img));
  assert(row >= 0 && row < img->rows);
  assert(col >= 0 && col < img->cols);

  return img->data[row * img->cols + col];
}

void
image_set(
  image* img,
  int row,
  int col,
  color c)
{
  assert(image_is_valid(img));
  assert(row >= 0 && row < img->rows);
  assert(col >= 0 && col < img->cols);
  assert(color_is_valid(&c));

  img->data[row * img->cols + col].r = c.r;
  img->data[row * img->cols + col].g = c.g;
  img->data[row * img->cols + col].b = c.b;
  img->data[row * img->cols + col].a = c.a;
}


image*
image_from_ppm_t(
  const char* ppm_path)
{
  assert(ppm_path != NULL);
  assert(ppm_path[0] != '\0');

  #define TEXT_LINE_MAX_SIZE 1024

  image*  new_img;
  FILE*   ppm_file;
  char*   text_line;
  int     width;
  int     height;
  int     color_r;
  int     color_g;
  int     color_b;
  int     i;
  int     j;

  ppm_file = fopen(ppm_path,"rt");
  text_line = malloc(sizeof(char) * TEXT_LINE_MAX_SIZE);

  /* Jump first two ines */

  memset(text_line,0,TEXT_LINE_MAX_SIZE);
  fgets(text_line,TEXT_LINE_MAX_SIZE,ppm_file);
  memset(text_line,0,TEXT_LINE_MAX_SIZE);
  fgets(text_line,TEXT_LINE_MAX_SIZE,ppm_file);
  fscanf(ppm_file,"%d %d",&width,&height);
  fscanf(ppm_file,"%*d");

  new_img = image_blank(height,width,(color){0,0,0,1});

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      fscanf(ppm_file,"%d %d %d",&color_r,&color_g,&color_b);
      image_set(new_img,i,j,(color){color_r/(float)255,color_g/(float)255,color_b/(float)255,1});
    }
  }

  free(text_line);
  fclose(ppm_file);

  return new_img;
}
