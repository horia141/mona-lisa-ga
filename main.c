#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ext/snt/utils.h"
#include "ext/snt/color.h"
#include "ext/snt/rectangle.h"
#include "ext/snt/image.h"
#include "ext/snt/driver.h"

#include "gastuff.h"

static struct {
  bool          console_mode;
  image*        target;
  int           max_iteration;
  int           indi_count;
  int           gene_count;
  int           mu;
  int           workers_cnt;
  int           evolve_time;
  int           grid_rows;
  int           grid_cols;
  bool          exit_on_stop;
  int           draw_skip;
} config;

static struct {
  population*   pop;
  driver*       drv;
  tquad*        target;
  tquad*        best;
  tquad***      grid;
  int           curr_iteration;
} state;

int
app_update_cb()
{
  const image*  work_image;
  int           i;
  int           j;

  if (state.curr_iteration < config.max_iteration) {
    population_evolve(state.pop);

    if (state.curr_iteration % (config.draw_skip + 1) == 0) {
      work_image = population_get_best_image(state.pop);
      tquad_update_texture(state.best,work_image);

      for (i = 0; i < config.grid_rows; i++) {
	for (j = 0; j < config.grid_cols; j++) {
	  work_image = population_get_curr_image(state.pop,i * config.grid_cols + j);
	  tquad_update_texture(state.grid[i][j],work_image);
	}
      }
    }

    state.curr_iteration += 1;

    return 1;
  } else {
    if (config.exit_on_stop) {
      return 0;
    } else {
      return 1;
    }
  }
}

void
app_init_display()
{
  float      grid_x = 0.1;
  float      grid_y = 0.55;
  float      grid_w = 0.8;
  float      grid_h = 0.35;
  float      cell_w = grid_w / config.grid_cols;
  float      cell_h = grid_h / config.grid_rows;
  float      padding_x = 0.05 * cell_w;
  float      padding_y = 0.05 * cell_h;
  rectangle  cell_geom;
  int        i;
  int        j;

  state.target = driver_tquad_make_image(state.drv,&(rectangle){0.1,0.1,0.35,0.35},config.target);
  state.best = driver_tquad_make_image(state.drv,&(rectangle){0.55,0.1,0.35,0.35},config.target);
  state.grid = malloc(sizeof(tquad**) * config.grid_rows);
  state.grid[0] = malloc(sizeof(tquad*) * config.grid_rows * config.grid_cols);

  for (i = 0; i < config.grid_rows; i++) {
    state.grid[i] = state.grid[0] + i * config.grid_cols;
  }

  for (i = 0; i < config.grid_rows; i++) {
    for (j = 0; j < config.grid_cols; j++) {
      cell_geom = rectangle_make_xywh(grid_x + j*cell_w + padding_x,
				      grid_y + i*cell_h + padding_y,
				      cell_w - 2*padding_x,
				      cell_h - 2*padding_y);
      
      state.grid[i][j] = driver_tquad_make_image(state.drv,&cell_geom,config.target);
    }
  }
}

void
app_free_display()
{
  free(state.grid[0]);
  free(state.grid);
}

void
con_run()
{
  printf("Not Started\n");

  while (state.curr_iteration < config.max_iteration) {
    population_evolve(state.pop);

    if (state.curr_iteration % (config.draw_skip + 1) == 0) {
      printf("Running: Iteration #%d/%d\n",state.curr_iteration + 1,config.max_iteration);
    }

    state.curr_iteration += 1;
  }

  printf("Stopped\n");
}

int
main(
  int argc,
  char** argv)
{
  srandom(time(NULL));

  config.console_mode = false;
  config.target = image_from_ppm_t("Mondrian.ppm");
  config.max_iteration = 300;
  config.indi_count = 32;
  config.gene_count = 32; 
  config.mu = 4;
  config.workers_cnt = 2;
  config.evolve_time = 10;
  config.grid_rows = 1;
  config.grid_cols = 4;
  config.exit_on_stop = true;
  config.draw_skip = 0;

  state.pop = population_random(config.indi_count,config.gene_count,config.mu,config.target,config.workers_cnt);
  state.curr_iteration = 0;

  if (!config.console_mode) {
    state.drv = driver_make(app_update_cb,"MonaLisa Genetic Algorithm",&(rectangle){100,100,600,600},config.evolve_time);
    
    app_init_display();

    driver_start(state.drv);
    driver_free(state.drv);

    app_free_display();
  } else {
    state.drv = NULL;
    
    con_run();
  }

  population_free(state.pop);
  image_free(config.target);

  return 0;
}
