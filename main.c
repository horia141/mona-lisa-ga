#include <stdio.h>
#include <string.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "common.h"
#include "image.h"
#include "gastuff.h"

static struct {
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
  struct {
    int         window_id;
    geometry    target_place;
    int         target_tid;
    geometry    best_place;
    int         best_tid;
    geometry**  grid_places;
    int**       grid_tids;
  } display;
} config;

static struct {
  population*   pop;
  int           curr_iteration;
} state;

void
app_update_cb(
  int __unused)
{
  #define WINDOW_TITLE_SIZE 256
  static char        window_title[WINDOW_TITLE_SIZE + 1];
  static float*      work_texture = NULL;
  const image*       work_image;
  int                i;
  int                j;

  if (state.curr_iteration < config.max_iteration) {
    if (work_texture == NULL) {
      work_texture = malloc(sizeof(float) * image_get_rows(config.target) * image_get_cols(config.target) * 4);
    }

    population_evolve(state.pop);

    if (state.curr_iteration % (config.draw_skip + 1) == 0) {
      /* Upload Best new texture. */

      work_image = population_get_best_image(state.pop);
      image_make_texture_a(work_image,work_texture);

      glBindTexture(GL_TEXTURE_2D,config.display.best_tid);
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,
		   image_get_cols(work_image),image_get_rows(work_image),
		   0,GL_RGBA,GL_FLOAT,work_texture);

      /* Upload Grid new textures. */

      for (i = 0; i < config.grid_rows; i++) {
	for (j = 0; j < config.grid_cols; j++) {
	  work_image = population_get_curr_image(state.pop,i * config.grid_cols + j);
	  image_make_texture_a(work_image,work_texture);

	  glBindTexture(GL_TEXTURE_2D,config.display.grid_tids[i][j]);
	  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,
		       image_get_cols(work_image),image_get_rows(work_image),
		       0,GL_RGBA,GL_FLOAT,work_texture);
	}
      }

      memset(window_title,0,WINDOW_TITLE_SIZE + 1);
      snprintf(window_title,WINDOW_TITLE_SIZE,"Running: Iteration #%d/%d",state.curr_iteration + 1,config.max_iteration);
      glutSetWindowTitle(window_title);
      glutPostRedisplay();
    }

    glutTimerFunc(config.evolve_time,app_update_cb,0);
    state.curr_iteration += 1;
  } else {
    free(work_texture);

    memset(window_title,0,WINDOW_TITLE_SIZE + 1);
    snprintf(window_title,WINDOW_TITLE_SIZE,"Stopped",state.curr_iteration + 1);
    glutSetWindowTitle(window_title);

    if (config.exit_on_stop) {
      glutDestroyWindow(config.display.window_id);
    }
  }
}

void
app_display_cb()
{
  int  i;
  int  j;

  glClear(GL_COLOR_BUFFER_BIT);

  /* Draw Target. */

  glBindTexture(GL_TEXTURE_2D,config.display.target_tid);

  glBegin(GL_QUADS);
  glColor3f(1,0,0);
  glTexCoord2f(0,1);
  glVertex2f(config.display.target_place.x,
	     config.display.target_place.y);
  glTexCoord2f(1,1);
  glVertex2f(config.display.target_place.x+
	     config.display.target_place.w,
	     config.display.target_place.y);
  glTexCoord2f(1,0);
  glVertex2f(config.display.target_place.x+
	     config.display.target_place.w,
	     config.display.target_place.y+
	     config.display.target_place.h);
  glTexCoord2f(0,0);
  glVertex2f(config.display.target_place.x,
	     config.display.target_place.y+
	     config.display.target_place.h);
  glEnd();

  /* Draw Best. */

  glBindTexture(GL_TEXTURE_2D,config.display.best_tid);

  glBegin(GL_QUADS);
  glColor3f(0,1,0);
  glTexCoord2f(0,1);
  glVertex2f(config.display.best_place.x,
	     config.display.best_place.y);
  glTexCoord2f(1,1);
  glVertex2f(config.display.best_place.x+
	     config.display.best_place.w,
	     config.display.best_place.y);
  glTexCoord2f(1,0);
  glVertex2f(config.display.best_place.x+
	     config.display.best_place.w,
	     config.display.best_place.y+
	     config.display.best_place.h);
  glTexCoord2f(0,0);
  glVertex2f(config.display.best_place.x,
	     config.display.best_place.y+
	     config.display.best_place.h);
  glEnd();

  /* Draw Grid. */

  for (i = 0; i < config.grid_rows; i++) {
    for (j = 0; j < config.grid_cols; j++) {
      glBindTexture(GL_TEXTURE_2D,config.display.grid_tids[i][j]);

      glBegin(GL_QUADS);
      glColor3f(unirandom_f(0,1),unirandom_f(0,1),0);
      glTexCoord2f(0,1);
      glVertex2f(config.display.grid_places[i][j].x,
		 config.display.grid_places[i][j].y);
      glTexCoord2f(1,1);
      glVertex2f(config.display.grid_places[i][j].x+
		 config.display.grid_places[i][j].w,
		 config.display.grid_places[i][j].y);
      glTexCoord2f(1,0);
      glVertex2f(config.display.grid_places[i][j].x+
		 config.display.grid_places[i][j].w,
		 config.display.grid_places[i][j].y+
		 config.display.grid_places[i][j].h);
      glTexCoord2f(0,0);
      glVertex2f(config.display.grid_places[i][j].x,
		 config.display.grid_places[i][j].y+
		 config.display.grid_places[i][j].h);
      glEnd();
    }
  }

  glutSwapBuffers();
}

void
app_init_display(
  int*  argc,
  char** argv)
{
  float*  target_texture;
  float   grid_x = 0.1;
  float   grid_y = 0.55;
  float   grid_w = 0.8;
  float   grid_h = 0.35;
  float   cell_w = grid_w / config.grid_cols;
  float   cell_h = grid_h / config.grid_rows;
  float   padding_x = 0.05 * cell_w;
  float   padding_y = 0.05 * cell_h;
  int     i;
  int     j;

  glutInit(argc,argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(100,100);
  config.display.window_id = glutCreateWindow("Not Started");
  glutDisplayFunc(app_display_cb);

  glClearColor(0,0,0,0);
  glLoadIdentity();
  glOrtho(-1,1,-1,1,-1,1);
  glScalef(2,2,1);
  glTranslatef(-0.5,0.5,0);
  glRotatef(180,1,0,0);

  glEnable(GL_TEXTURE_2D);

  /* Initialize Target fields. */

  config.display.target_place.x = 0.1;
  config.display.target_place.y = 0.1;
  config.display.target_place.w = 0.35;
  config.display.target_place.h = 0.35;

  target_texture = image_make_texture(config.target);

  glGenTextures(1,&config.display.target_tid);
  glBindTexture(GL_TEXTURE_2D,config.display.target_tid);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,
	       image_get_cols(config.target),image_get_rows(config.target),
	       0,GL_RGBA,GL_FLOAT,target_texture);

  free(target_texture);

  /* Initialize Best fields. */

  config.display.best_place.x = 0.55;
  config.display.best_place.y = 0.1;
  config.display.best_place.w = 0.35;
  config.display.best_place.h = 0.35;

  glGenTextures(1,&config.display.best_tid);
  glBindTexture(GL_TEXTURE_2D,config.display.best_tid);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

  /* Initialize Grid fields. */

  config.display.grid_places = malloc(sizeof(geometry*) * config.grid_rows);
  config.display.grid_places[0] = malloc(sizeof(geometry) * config.grid_rows * config.grid_cols);

  for (i = 0; i < config.grid_rows; i++) {
    config.display.grid_places[i] = config.display.grid_places[0] + i*config.grid_cols;
  }

  config.display.grid_tids = malloc(sizeof(int*) * config.grid_rows);
  config.display.grid_tids[0] = malloc(sizeof(int) * config.grid_rows * config.grid_cols);

  for (i = 0; i < config.grid_rows; i++) {
    config.display.grid_tids[i] = config.display.grid_tids[0] + i*config.grid_cols;
  }

  glGenTextures(config.grid_rows * config.grid_cols,
		config.display.grid_tids[0]);

  for (i = 0; i < config.grid_rows; i++) {
    for (j = 0; j < config.grid_cols; j++) {
      config.display.grid_places[i][j].x = grid_x + j*cell_w + padding_x;
      config.display.grid_places[i][j].y = grid_y + i*cell_h + padding_y;
      config.display.grid_places[i][j].w = cell_w - 2*padding_x;
      config.display.grid_places[i][j].h = cell_h - 2*padding_y;

      glBindTexture(GL_TEXTURE_2D,config.display.grid_tids[i][j]);
      glPixelStorei(GL_UNPACK_ALIGNMENT,1);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
      glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    }
  }
}

void
app_run()
{
  glutTimerFunc(config.evolve_time,app_update_cb,0);
  glutMainLoop();
}

int
main(
  int argc,
  char** argv)
{
  srandom(time(NULL));

  config.target = image_from_ppm_t("Mondrian.ppm");
  config.max_iteration = 300;
  config.indi_count = 32;
  config.gene_count = 32;
  config.mu = 4;
  config.workers_cnt = 2;
  config.evolve_time = 10;
  config.grid_rows = 2;
  config.grid_cols = 8;
  config.exit_on_stop = true;
  config.draw_skip = 49;

  state.pop = population_random(config.indi_count,config.gene_count,config.mu,
				config.target,config.workers_cnt);
  state.curr_iteration = 0;

  app_init_display(&argc,argv);
  app_run();

  population_free(state.pop);
  image_free(config.target);
  free(config.display.grid_places[0]);
  free(config.display.grid_places);
  free(config.display.grid_tids[0]);
  free(config.display.grid_tids);

  return 0;
}
