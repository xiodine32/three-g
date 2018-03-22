#ifndef SRC_FONT_H_
#define SRC_FONT_H_

#include "all.h"

void font_init(GLuint texture_id,
    int sprite_width, int sprite_height,
    int image_width, int image_height);

void font_draw_left(double y, double x, int size, const char *fmt, ...);

#endif  // SRC_FONT_H_
