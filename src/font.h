#ifndef __FONT_H
#define __FONT_H

#include "all.h"

void font_init(GLuint texture_id, int sprite_width, int sprite_height, int image_width, int image_height);
void font_draw(double y, double x, int size, const char *fmt, ...);

#endif