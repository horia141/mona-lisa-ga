#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "gastuff.h"

typedef struct _indi_desc  indi_desc;


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


individual*
individual_copy(
  individual* dst,
  const individual* src)
{
  assert(individual_is_valid(dst));
  assert(individual_is_valid(src));
  assert(dst->gene_cnt == src->gene_cnt);

  int  i;

  for (i = 0; i < dst->gene_cnt; i++) {
    rectangle_copy(&dst->genes[i],&src->genes[i]);
  }

  return dst;
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

  new_img = image_blank(rows,cols,(color){0,0,0,1});
  
  return individual_to_image_a(indi,new_img);
}

image*
individual_to_image_a(
  const individual* indi,
  image* image)
{
  assert(individual_is_valid(indi));
  assert(image_is_valid(image));

  color   color_sum;
  int     rows;
  int     cols;
  int     intersect_cnt;
  int*    gene_x_scaled;
  int*    gene_y_scaled;
  int*    gene_w_scaled;
  int*    gene_h_scaled;
  int     i;
  int     j;
  int     k;

  rows = image_get_rows(image);
  cols = image_get_cols(image);

  gene_x_scaled = malloc(sizeof(int) * indi->gene_cnt);
  gene_y_scaled = malloc(sizeof(int) * indi->gene_cnt);
  gene_w_scaled = malloc(sizeof(int) * indi->gene_cnt);
  gene_h_scaled = malloc(sizeof(int) * indi->gene_cnt);

  for (k = 0; k < indi->gene_cnt; k++) {
    gene_x_scaled[k] = clamp_i((int)floorf(indi->genes[k].geometry.x * cols),0,cols);
    gene_y_scaled[k] = clamp_i((int)floorf(indi->genes[k].geometry.y * rows),0,rows);
    gene_w_scaled[k] = clamp_i((int)floorf(indi->genes[k].geometry.w * cols),0,cols - gene_x_scaled[k]);
    gene_h_scaled[k] = clamp_i((int)floorf(indi->genes[k].geometry.h * rows),0,rows - gene_y_scaled[k]);
  }

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      color_sum.r = 0;
      color_sum.g = 0;
      color_sum.b = 0;
      color_sum.a = 0;
      intersect_cnt = 0;

      for (k = 0; k < indi->gene_cnt; k++) {
	if (i >= gene_y_scaled[k] && i <= gene_y_scaled[k] + gene_h_scaled[k] &&
	    j >= gene_x_scaled[k] && j <= gene_x_scaled[k] + gene_w_scaled[k]) {
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

	image_set(image,i,j,color_sum);
      }
    }
  }

  free(gene_x_scaled);
  free(gene_y_scaled);
  free(gene_w_scaled);
  free(gene_h_scaled);
  
  return image;
}

individual*
individual_crossover(
  individual* target,
  const individual* parent1,
  const individual* parent2,
  const crossmask* mask)
{
  assert(individual_is_valid(target));
  assert(individual_is_valid(parent1));
  assert(individual_is_valid(parent2));
  assert(crossmask_is_valid(mask));
  assert(target->gene_cnt == parent1->gene_cnt);
  assert(target->gene_cnt == parent2->gene_cnt);
  assert(target->gene_cnt == crossmask_get_flag_cnt(mask));

  int  i;

  for (i = 0; i < target->gene_cnt; i++) {
    if (crossmask_get(mask,i) == true) {
      rectangle_copy(&target->genes[i],&parent1->genes[i]);
    } else {
      rectangle_copy(&target->genes[i],&parent2->genes[i]);
    }
  }

  return target;
}

individual*
individual_mutate(
  individual* indi)
{
  assert(individual_is_valid(indi));

  int  modif_gene;

  modif_gene = unirandom_i(0,indi->gene_cnt);

  indi->genes[modif_gene].geometry.x = clamp_f(indi->genes[modif_gene].geometry.x + unirandom_f(-0.05,0.05),0,0.90);
  indi->genes[modif_gene].geometry.y = clamp_f(indi->genes[modif_gene].geometry.y + unirandom_f(-0.05,0.05),0,0.90);
  indi->genes[modif_gene].geometry.w = clamp_f(indi->genes[modif_gene].geometry.w + unirandom_f(-0.05,0.05),0.05,1 - indi->genes[modif_gene].geometry.x);
  indi->genes[modif_gene].geometry.h = clamp_f(indi->genes[modif_gene].geometry.h + unirandom_f(-0.05,0.05),0.05,1 - indi->genes[modif_gene].geometry.y);
  indi->genes[modif_gene].color.r = clamp_f(indi->genes[modif_gene].color.r + unirandom_f(-0.05,0.05),0,1);
  indi->genes[modif_gene].color.g = clamp_f(indi->genes[modif_gene].color.g + unirandom_f(-0.05,0.05),0,1);
  indi->genes[modif_gene].color.b = clamp_f(indi->genes[modif_gene].color.b + unirandom_f(-0.05,0.05),0,1);
  indi->genes[modif_gene].color.a = 1;

  return indi;
}

struct _indi_desc
{
  individual*  indi;
  image*       image;
  float        score;
};

struct _population
{
  int           gene_cnt;
  int           mu;
  const image*  target;
  indi_desc     best;
  int           indi_desc_cnt;
  indi_desc*    indi_descs;

};


static float
_population_calc_score(
  const image* indi_image,
  const image* target)
{
  assert(image_is_valid(indi_image));
  assert(target);
  assert(image_get_rows(indi_image) == image_get_rows(target));
  assert(image_get_cols(indi_image) == image_get_cols(target));

  float  score;
  color  tmp_a;
  color  tmp_b;
  int    rows;
  int    cols;
  int    i;
  int    j;

  score = 0;
  rows = image_get_rows(indi_image);
  cols = image_get_cols(indi_image);

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      tmp_a = image_get(indi_image,i,j);
      tmp_b = image_get(target,i,j);
      score += color_distance(&tmp_a,&tmp_b);
    }
  }

  return score;
}

static int
_population_compare_indi_desc(
  const void* a,
  const void* b)
{
  assert(a != NULL);
  assert(b != NULL);

  indi_desc*  t_a = (indi_desc*)a;
  indi_desc*  t_b = (indi_desc*)b;

  if (t_a->score > t_b->score) {
    return 1;
  } else if (t_a->score < t_b->score) {
    return -1;
  } else {
    return 0;
  }

  return 0;
}


population*
population_random(
  int indi_cnt,
  int gene_cnt,
  int mu,
  const image* target)
{
  assert(indi_cnt > 0);
  assert(gene_cnt > 0);
  assert(mu > 0);
  assert(image_is_valid(target));
  assert(indi_cnt % mu == 0);

  population*  new_pop;
  int          target_rows;
  int          target_cols;
  int          i;
  
  target_rows = image_get_rows(target);
  target_cols = image_get_cols(target);

  new_pop = malloc(sizeof(population));

  new_pop->gene_cnt = gene_cnt;
  new_pop->mu = mu;
  new_pop->target = target;
  new_pop->best.indi = individual_random(gene_cnt);
  new_pop->best.image = individual_to_image(new_pop->best.indi,target_rows,target_cols);
  new_pop->indi_desc_cnt = indi_cnt;
  new_pop->indi_descs = malloc(sizeof(indi_desc) * indi_cnt);

  for (i = 0; i < indi_cnt; i++) {
    new_pop->indi_descs[i].indi = individual_random(gene_cnt);
    new_pop->indi_descs[i].image = individual_to_image(new_pop->indi_descs[i].indi,target_rows,target_cols);
    new_pop->indi_descs[i].score = _population_calc_score(new_pop->indi_descs[i].image,target);
  }

  qsort(new_pop->indi_descs,new_pop->indi_desc_cnt,sizeof(indi_desc),_population_compare_indi_desc);

  individual_copy(new_pop->best.indi,new_pop->indi_descs[0].indi);
  image_copy(new_pop->best.image,new_pop->indi_descs[0].image);
  new_pop->best.score = new_pop->indi_descs[0].score;

  return new_pop;
}

void
population_free(
  population* pop)
{
  assert(population_is_valid(pop));

  int  i;

  for (i = 0; i < pop->indi_desc_cnt; i++) {
    individual_free(pop->indi_descs[i].indi);
    image_free(pop->indi_descs[i].image);
  }

  free(pop->indi_descs);

  pop->gene_cnt = -1;
  pop->mu = -1;
  pop->target = NULL;
  pop->indi_desc_cnt = -1;
  pop->indi_descs = NULL;

  free(pop);
}


bool
population_is_valid(
  const population* pop)
{
  int  i;

  if (pop == NULL) {
    return false;
  }

  if (pop->gene_cnt < 1) {
    return false;
  }

  if (pop->mu < 1) {
    return false;
  }

  if (pop->indi_desc_cnt % pop->mu != 0) {
    return false;
  }

  if (pop->indi_desc_cnt < 1) {
    return false;
  }

  if (pop->indi_descs == NULL) {
    return false;
  }

  for (i = 0; i< pop->indi_desc_cnt; i++) {
    if (!individual_is_valid(pop->indi_descs[i].indi)) {
      return false;
    }

    if (!image_is_valid(pop->indi_descs[i].image)) {
      return false;
    }

    if (pop->indi_descs[i].score < 0) {
      return false;
    }
  }

  return true;
}


population*
population_evolve(
  population* pop)
{
  assert(population_is_valid(pop));

  int         mul_factor;
  int         new_id;
  int         i;
  int         j;

  mul_factor = pop->indi_desc_cnt / pop->mu;

  for (i = 1; i < mul_factor; i++) {
    for (j = 0; j < pop->mu; j++) {
      new_id = i * pop->mu + j;

      individual_copy(pop->indi_descs[new_id].indi,pop->indi_descs[j].indi);
      individual_mutate(pop->indi_descs[new_id].indi);
      individual_to_image_a(pop->indi_descs[new_id].indi,pop->indi_descs[new_id].image);
      pop->indi_descs[new_id].score = _population_calc_score(pop->indi_descs[new_id].image,pop->target);
    }
  }

  for (i = 0; i < pop->mu; i++) {
      individual_mutate(pop->indi_descs[i].indi);
      individual_to_image_a(pop->indi_descs[i].indi,pop->indi_descs[i].image);
      pop->indi_descs[i].score = _population_calc_score(pop->indi_descs[i].image,pop->target);
  }

  qsort(pop->indi_descs,pop->indi_desc_cnt,sizeof(indi_desc),_population_compare_indi_desc);

  if (pop->indi_descs[0].score < pop->best.score) {
    individual_copy(pop->best.indi,pop->indi_descs[0].indi);
    image_copy(pop->best.image,pop->indi_descs[0].image);
    pop->best.score = pop->indi_descs[0].score;
  }

  return pop;
}


const image*
population_get_best_image(
  const population* pop)
{
  assert(population_is_valid(pop));

  return pop->best.image;
}

const image*
population_get_curr_image(
  const population* pop,
  int indi_id)
{
  assert(population_is_valid(pop));
  assert(indi_id >= 0 && indi_id < pop->indi_desc_cnt);

  return pop->indi_descs[indi_id].image;
}


struct _crossmask
{
  int   flag_cnt;
  bool  flags[];
};

static crossmask*
_crossmask_null(
  int flag_cnt)
{
  assert(flag_cnt > 0);

  crossmask*  mask;

  mask = malloc(sizeof(crossmask) + sizeof(bool) * flag_cnt);
  mask->flag_cnt = flag_cnt;

  return mask;
}


crossmask*
crossmask_onecut(
  int flag_cnt)
{
  assert(flag_cnt > 0);

  crossmask*  new_mask;
  int         cut_point;
  int         i;

  new_mask = _crossmask_null(flag_cnt);
  cut_point = unirandom_i(1,flag_cnt);

  for (i = 0; i < cut_point; i++) {
    new_mask->flags[i] = true;
  }

  for (i = cut_point; i < flag_cnt; i++) {
    new_mask->flags[i] = false;
  }

  return new_mask;
}

crossmask*
crossmask_uniform(
  int flag_cnt)
{
  assert(flag_cnt > 0);

  crossmask*  new_mask;
  int         i;

  new_mask = _crossmask_null(flag_cnt);

  for (i = 0; i < flag_cnt; i++) {
    new_mask->flags[i] = unirandom_b();
  }

  return new_mask;
}

void
crossmask_free(
  crossmask* mask)
{
  assert(crossmask_is_valid(mask));

  mask->flag_cnt = -1;

  free(mask);
}


bool
crossmask_is_valid(
  const crossmask* mask)
{
  if (mask == NULL) {
    return false;
  }

  if (mask->flag_cnt < 1) {
    return false;
  }

  return true;
}


int
crossmask_get_flag_cnt(
  const crossmask* mask)
{
  assert(crossmask_is_valid(mask));

  return mask->flag_cnt;
}

bool
crossmask_get(
  const crossmask* mask,
  int i)
{
  assert(crossmask_is_valid(mask));
  assert(i >= 0 && i < mask->flag_cnt);

  return mask->flags[i];
}
