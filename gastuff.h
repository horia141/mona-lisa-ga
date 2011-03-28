#ifndef _GASTUFF_H
#define _GASTUFF_H

#include <stdbool.h>

#include "common.h"
#include "image.h"

typedef struct _individual  individual;
typedef struct _population  population;

struct _individual
{
  int         gene_cnt;
  rectangle*  genes;
};

struct _population
{
  int          indi_cnt;
  individual*  indis;
};

bool    individual_is_valid(const individual* indi);

image*  individual_to_image(const individual* indi, int rows, int cols);

bool    population_is_valid(const population* pop);

#endif
