#ifndef _GASTUFF_H
#define _GASTUFF_H

#include <stdbool.h>

#include "common.h"
#include "image.h"

typedef struct _individual  individual;
typedef struct _population  population;
typedef struct _crossmask   crossmask;

individual*  individual_random(int gene_cnt);
void         individual_free(individual* indi);

bool         individual_is_valid(const individual* indi);

image*       individual_to_image(const individual* indi, int rows, int cols);
individual*  individual_crossover(individual* target, const individual* parent1, const individual* parent2, const crossmask* mask);

population*  population_random(int indi_cnt, int gene_cnt, int lambda, int mu, const image* target);
void         population_free(population* pop);

bool         population_is_valid(const population* pop);

population*  population_evolve(population* pop);
const image* population_get_cached_image(const population* pop, int indi_id);

crossmask*   crossmask_onecut(int flags_cnt);
crossmask*   crossmask_uniform(int flags_cnt);
void         crossmask_free(crossmask* mask);

bool         crossmask_is_valid(const crossmask* mask);

int          crossmask_get_flag_cnt(const crossmask* mask);
bool         crossmask_get(const crossmask* mask, int i);

#endif
