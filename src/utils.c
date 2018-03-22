#include "utils.h"
#include <string.h>
#include "config.h"
#include "font.h"

static char file_name[256] = {0};
static int scene_update_frame;


#ifndef DEBUG
    #define D_RETURN return
#else
    #define D_RETURN
#endif

void d_scene_init(const char *file) {
    D_RETURN;
    snprintf(file_name, sizeof(file_name), "%s", file);
}
void d_scene_update(int scene_frame) {
    D_RETURN;
    scene_update_frame = scene_frame;
}
void d_scene(int scene_frame, int scene_time_delta) {
    D_RETURN;
    font_draw_left(0, 0, 16, "[%s] draw: %4d (%dms) - update: %4d (%dms)",
        file_name,
        scene_frame, scene_time_delta,
        scene_update_frame, (int)(1000 / ENGINE_TICK));
}
