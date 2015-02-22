#ifndef _DRIVER_H
#define _DRIVER_H

#include "utils.h"
#include "color.h"
#include "rectangle.h"
#include "image.h"

typedef int           (*driver_frame_cb)(void);
typedef struct _driver  driver;
typedef struct _tquad   tquad;

driver*           driver_make(driver_frame_cb frame_cb, const char* title, const rectangle* geometry, int ms_per_frame);
void              driver_free(driver* drv);
	      
bool              driver_is_valid(const driver* drv);
	      
driver*           driver_start(driver* drv);
tquad*            driver_tquad_make_color(driver* drv, const rectangle* geometry, int rows, int cols, const color* color);
tquad*            driver_tquad_make_image(driver* drv, const rectangle* geometry, const image* texture);
tquad*            driver_tquad_make_copy(driver* drv, const tquad* src);
void              driver_tquad_free(driver* drv, tquad* tq);

const char*       driver_get_title(const driver* drv);
void              driver_set_title(driver* drv, const char* format,...);
void              driver_move_to(driver* drv, float x, float y);
void              driver_move_by(driver* drv, float x, float y);
void              driver_resize_to(driver* drv, float w, float h);
void              driver_resize_by(driver* drv, float w, float h);
	      
bool              tquad_is_valid(const tquad* tq);

void              tquad_update_texture(tquad* tq, const image* img);
void              tquad_move_to(tquad* tq, float x, float y);
void              tquad_move_by(tquad* tq, float dx, float dy);
void              tquad_resize_to(tquad* tq, float w, float h);
void              tquad_resize_by(tquad* tq, float dw, float dh);
void              tquad_show(tquad* tq);
void              tquad_hide(tquad* tq);
void              tquad_visiflip(tquad* tq);
const rectangle*  tquad_get_geometry(const tquad* tq);
const image*      tquad_get_texture(const tquad* tq);
const color*      tquad_texture_get(const tquad* tq, int row, int col);
void              tquad_texture_set(tquad* tq, int row, int col, const color* c);

#endif
