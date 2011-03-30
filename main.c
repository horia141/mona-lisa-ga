#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "common.h"
#include "image.h"
#include "gastuff.h"

void
view_display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D,42);

  glBegin(GL_QUADS);
  glTexCoord2f(0,0);glVertex2f(-0.5,-0.5);
  glTexCoord2f(0,1);glVertex2f(-0.5, 0.5);
  glTexCoord2f(1,1);glVertex2f( 0.5, 0.5);
  glTexCoord2f(1,0);glVertex2f( 0.5,-0.5);
  glEnd();

  glFlush();
}

void
view_init()
{
  individual* indi;
  image*  img;
  float*  texture;

  indi = individual_random(50);
  img = individual_to_image(indi,64,64);
  texture = image_make_texture(img);

  glClearColor(0,0,0,0);
  glColor3f(1,1,1);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1,1,-1,1,-1,1);
  
  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D,42);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image_get_cols(img),image_get_rows(img),0,GL_RGBA,GL_FLOAT,texture);

  free(texture);
  image_free(img);
  individual_free(indi);
}

void
view_update(
 int value)
{
  printf("hello\n\n");

  if (value == 1) {
    glutTimerFunc(1000,view_update,0);
  }
}

int
main(int argc, char** argv)
{
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(0,0);
  glutCreateWindow("simple");
  glutDisplayFunc(view_display);
  glutTimerFunc(1000,view_update,1);

  view_init();
  glutMainLoop();

  return 0;
}
