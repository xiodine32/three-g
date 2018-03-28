#ifndef SRC_SCENE_H_
#define SRC_SCENE_H_

#include "all.h"

typedef void (*scene_draw_callback)(int scene_frame, int scene_time_delta);
typedef void (*scene_update_callback)(int scene_frame);
typedef void (*scene_helper)();

struct scene_t {
    scene_helper load;
    scene_draw_callback draw;
    scene_update_callback update;
    scene_helper unload;
};

void scene_draw();
void scene_update();
void scene_new(int scene_id, const scene_t &scene);
void scene_set(int scene_id);
void scene_animate_in(int scene_id,
    GLuint time_interval = SCENE_ANIMATION_DEFAULT);
void scene_animate_out(int scene_id,
    GLuint time_interval = SCENE_ANIMATION_DEFAULT);
int scene_get_id();

#endif  // SRC_SCENE_H_
