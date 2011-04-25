#ifndef _GASTUFF_H
#define _GASTUFF_H

#include <stdbool.h>

#include "ext/snt/utils.h"
#include "ext/snt/color.h"
#include "ext/snt/rectangle.h"
#include "ext/snt/image.h"

typedef struct _gene        gene;
typedef struct _individual  individual;
typedef struct _population  population;
typedef struct _crossmask   crossmask;

struct _gene
{
  rectangle  geometry;
  color      color;
};

gene*        gene_copy(gene* dst, const gene* src);
bool         gene_is_valid(const gene* r);


individual*  individual_random(int gene_cnt);
void         individual_free(individual* indi);

bool         individual_is_valid(const individual* indi);

individual*  individual_copy(individual* dst, const individual* src);

image*       individual_to_image(const individual* indi, int rows, int cols);
image*       individual_to_image_a(const individual* indi, image* image);
individual*  individual_crossover(individual* target, const individual* parent1, const individual* parent2, const crossmask* mask);
individual*  individual_mutate(individual* indi);

population*  population_random(int indi_cnt, int gene_cnt, int mu, const image* target, int worker_cnt);
void         population_free(population* pop);

bool         population_is_valid(const population* pop);

population*  population_evolve(population* pop);

const image* population_get_best_image(const population* pop);
const image* population_get_curr_image(const population* pop, int indi_id);

crossmask*   crossmask_onecut(int flags_cnt);
crossmask*   crossmask_uniform(int flags_cnt);
void         crossmask_free(crossmask* mask);

bool         crossmask_is_valid(const crossmask* mask);

int          crossmask_get_flag_cnt(const crossmask* mask);
bool         crossmask_get(const crossmask* mask, int i);

#endif
