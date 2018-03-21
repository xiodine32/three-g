#ifndef __SCENE_H
#define __SCENE_H

#include "all.h"

typedef void (*scene_draw_callback)(int scene_frame, int scene_time_delta);
typedef void (*scene_update_callback)(int scene_frame);
typedef void (*scene_helper)();

struct scene_t
{
    scene_helper load;
    scene_draw_callback draw;
    scene_update_callback update;
    scene_helper unload;
};

void scene_draw();
void scene_update();
void scene_new(int scene_id, scene_t &scene);
void scene_set(int scene_id);
int scene_get_id();

#endif