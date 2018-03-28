#include "all.h"
#include "scene.h"
#include "font.h"
#include "utils.h"

static void scene_about_load();
static void scene_about_update(int scene_frame);
static void scene_about_draw(int scene_frame, int scene_time_delta);
static void scene_about_unload();

void scene_init_about() {
    scene_t scn;
    scn.load = &scene_about_load;
    scn.unload = &scene_about_unload;
    scn.draw = &scene_about_draw;
    scn.update = &scene_about_update;

    scene_new(SCENE_ABOUT, scn);
}


void scene_about_load() {
    d("[scene_about_load]: stub\n");\
    d_scene_init(__FILE__);
}


void scene_about_update(int scene_frame) {
    if (mouse_pressed(true)) {
        scene_animate_out(SCENE_MAIN_MENU);
    }
}

void scene_about_draw(int scene_frame, int scene_time_delta) {
    font_draw_left(60, 60, 32, "About");
    font_draw_left(150, 60, 16, "Click anywhere to go back.");
}

void scene_about_unload() {
    d("[scene_about_unload]: stub\n");
}
