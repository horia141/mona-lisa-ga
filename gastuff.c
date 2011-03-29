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
    new_indi->genes[i].geometry.x = unirandom_f(0,0.94);
    new_indi->genes[i].geometry.y = unirandom_f(0,0.94);
    new_indi->genes[i].geometry.w = unirandom_f(0.05,1 - new_indi->genes[i].geometry.x);
    new_indi->genes[i].geometry.h = unirandom_f(0.05,1 - new_indi->genes[i].geometry.y);
    new_indi->genes[i].color.r = unirandom_f(0,1);
    new_indi->genes[i].color.g = unirandom_f(0,1);
    new_indi->genes[i].color.b = unirandom_f(0,1);
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


struct _population
{
  int           indi_cnt;
  individual**  indis;
};

population*
population_random(
  int indi_cnt,
  int gene_cnt)
{
  assert(indi_cnt > 0);

  population*  new_pop;
  int          i;

  new_pop = malloc(sizeof(population));

  new_pop->indi_cnt = indi_cnt;
  new_pop->indis = malloc(sizeof(individual*) * indi_cnt);

  for (i = 0; i < indi_cnt; i++) {
    new_pop->indis[i] = individual_random(gene_cnt);
  }

  return new_pop;
}

void
population_free(
  population* pop)
{
  assert(population_is_valid(pop));

  int  i;

  for (i = 0; i < pop->indi_cnt; i++) {
    individual_free(pop->indis[i]);
  }

  pop->indi_cnt = -1;
  pop->indis = NULL;
}


bool
population_is_valid(
  const population* pop)
{
  int  i;

  if (pop == NULL) {
    return false;
  }

  if (pop->indi_cnt < 1) {
    return false;
  }

  if (pop->indis == NULL) {
    return false;
  }

  for (i = 0; i< pop->indi_cnt; i++) {
    if (!individual_is_valid(pop->indis[i])) {
      return false;
    }
  }

  return true;
}


population*
population_evolve(
  population* pop)
{
}


struct _crossmask
{
  int   flag_cnt;
  bool  flags[];
};

static crossmask*
_crossmask_null(
  int flags_cnt)
{
  assert(flags_cnt > 0);

  crossmask*  mask;

  mask = malloc(sizeof(crossmask) + sizeof(bool) * flags_cnt);
  mask->flag_cnt = flags_cnt;

  return mask;
}


crossmask*
crossmask_onecut(
  int flags_cnt)
{
  assert(flags_cnt > 0);

  crossmask*  new_mask;
  int         cut_point;
  int         i;

  new_mask = _crossmask_null(flags_cnt);
  cut_point = unirandom_i(1,flags_cnt);

  for (i = 0; i < cut_point; i++) {
    new_mask->flags[i] = true;
  }

  for (i = cut_point; i < flags_cnt; i++) {
    new_mask->flags[i] = false;
  }

  return new_mask;
}

crossmask*
crossmask_uniform(
  int flags_cnt)
{
  assert(flags_cnt > 0);

  crossmask*  new_mask;
  int         i;

  new_mask = _crossmask_null(flags_cnt);

  for (i = 0; i < flags_cnt; i++) {
    new_mask->flags[i] = unirandom_b();
  }

  return new_mask;
}

void
crossmask_free(
  crossmask* mask)
{
  assert(crossmask_is_valid(mask));

  mask->flags_cnt = -1;

  free(mask);
}


bool
crossmask_is_valid(
  const crossmask* mask)
{
  if (mask == NULL) {
    return false;
  }

  if (mask->flags_cnt < 1) {
    return false;
  }

  return true;
}


int
crossmask_get_flags_cnt(
  const crossmask* mask)
{
  assert(crossmask_is_valid(mask));

  return mask->flags_cnt;
}

bool
crossmask_get(
  const crossmask* mask,
  int i)
{
  assert(crossmask_is_valid(mask));
  assert(i >= 0 && i < mask->flags_cnt);

  return mask->flags[i];
}
