#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

float*
load_ppm(const char* ppm_path, int* image_rows,int* image_cols)
{
  #define TEXT_LINE_MAX_SIZE 1024

  FILE*   ppm_file;
  char*   text_line;
  float*  image;
  int     rows;
  int     cols;
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
  fscanf(ppm_file,"%d %d",&rows,&cols);
  fscanf(ppm_file,"%*d");

  image = malloc(sizeof(float) * rows * cols * 3);

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      fscanf(ppm_file,"%d %d %d",&color_r,&color_g,&color_b);

      image[i * cols * 3 + j * 3 + 0] = (float)color_r / 255.0;
      image[i * cols * 3 + j * 3 + 1] = (float)color_g / 255.0;
      image[i * cols * 3 + j * 3 + 2] = (float)color_b / 255.0;
    }
  }

  free(text_line);
  fclose(ppm_file);

  *image_rows = rows;
  *image_cols = cols;

  return image;
}

void
view_display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D,42);

  glBegin(GL_QUADS);
  glTexCoord2f(0,1);glVertex2f(-0.5,-0.5);
  glTexCoord2f(0,0);glVertex2f(-0.5, 0.5);
  glTexCoord2f(1,0);glVertex2f( 0.5, 0.5);
  glTexCoord2f(1,1);glVertex2f( 0.5,-0.5);
  glEnd();

  glFlush();
}

void
view_init()
{
  int     image_rows;
  int     image_cols;
  float*  image = load_ppm("MonaLisa.ppm",&image_rows,&image_cols);

  glClearColor(0,0,0,0);
  glColor3f(1,1,1);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //  glRotatef(180,0,0,1);
  glOrtho(-1,1,-1,1,-1,1);
  
  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D,42);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1); //Might ned to be 4.
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image_rows,image_cols,0,GL_RGB,GL_FLOAT,image);
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

  view_init();
  glutMainLoop();

  return 0;
}
