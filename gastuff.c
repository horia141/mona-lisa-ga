#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "gastuff.h"

struct _individual
{
  int         gene_cnt;
  rectangle   genes[];
};

individual*
individual_random(
  int gene_cnt)
{
  assert(gene_cnt > 0);

  individual*  new_indi;
  int          i;

  new_indi = malloc(sizeof(individual) + sizeof(rectangle) * gene_cnt);

  new_indi->gene_cnt = gene_cnt;

  for (i = 0; i < gene_cnt; i++) {
    new_indi->genes[i].geometry.x = unirandom(0,0.94);
    new_indi->genes[i].geometry.y = unirandom(0,0.94);
    new_indi->genes[i].geometry.w = unirandom(0.05,1 - new_indi->genes[i].geometry.x);
    new_indi->genes[i].geometry.h = unirandom(0.05,1 - new_indi->genes[i].geometry.y);
    new_indi->genes[i].color.r = unirandom(0,1);
    new_indi->genes[i].color.g = unirandom(0,1);
    new_indi->genes[i].color.b = unirandom(0,1);
    new_indi->genes[i].color.a = 1;
  }

  return new_indi;
}

void
individual_free(
  individual* indi)
{
  assert(individual_is_valid(indi));

  indi->gene_cnt = -1;

  free(indi);
}

bool
individual_is_valid(
  const individual* indi)
{
  int  i;

  if (indi == NULL) {
    return false;
  }
  
  if (indi->gene_cnt < 1) {
    return false;
  }

  for (i = 0; i < indi->gene_cnt; i++) {
    if (!rectangle_is_valid(&indi->genes[i])) {
      return false;
    }
  }

  return true;
}

image*
individual_to_image(
  const individual* indi,
  int rows,
  int cols)
{
  assert(individual_is_valid(indi));
  assert(rows > 0);
  assert(cols > 0);

  image*  new_img;
  color   color_sum;
  int     intersect_cnt;
  int     gene_x_scaled;
  int     gene_y_scaled;
  int     gene_w_scaled;
  int     gene_h_scaled;
  int     i;
  int     j;
  int     k;

  new_img = image_blank(rows,cols,(color){0,0,0,1});
  
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      color_sum.r = 0;
      color_sum.g = 0;
      color_sum.b = 0;
      color_sum.a = 0;
      intersect_cnt = 0;

      for (k = 0; k < indi->gene_cnt; k++) {
	/* Should precompute these. */
	gene_x_scaled = clamp((int)floorf(indi->genes[k].geometry.x * cols),0,cols);
	gene_y_scaled = clamp((int)floorf(indi->genes[k].geometry.y * rows),0,rows);
	gene_w_scaled = clamp((int)floorf(indi->genes[k].geometry.w * cols),0,cols - gene_x_scaled);
	gene_h_scaled = clamp((int)floorf(indi->genes[k].geometry.h * rows),0,rows - gene_y_scaled);

	if (i >= gene_y_scaled && i <= gene_y_scaled + gene_h_scaled &&
	    j >= gene_x_scaled && j <= gene_x_scaled + gene_w_scaled) {
	  color_sum.r += indi->genes[k].color.r;
	  color_sum.g += indi->genes[k].color.g;
	  color_sum.b += indi->genes[k].color.b;
	  color_sum.a += indi->genes[k].color.a;
	  intersect_cnt += 1;
	}
      }

      if (intersect_cnt > 0) {
	color_sum.r /= (float)intersect_cnt;
	color_sum.g /= (float)intersect_cnt;
	color_sum.b /= (float)intersect_cnt;
	color_sum.a /= (float)intersect_cnt;

	image_set(new_img,i,j,color_sum);
      }
    }
  }

  return new_img;
}
