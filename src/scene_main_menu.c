#include "scene.h"

#include "font.h"
#include "utils.h"

static void scene_main_menu_load();
static void scene_main_menu_update(int scene_frame);
static void scene_main_menu_draw(int scene_frame, int scene_time_delta);
static void scene_main_menu_unload();

void scene_init_main_menu() {
    scene_t scn;
    scn.load = &scene_main_menu_load;
    scn.unload = &scene_main_menu_unload;
    scn.draw = &scene_main_menu_draw;
    scn.update = &scene_main_menu_update;

    scene_new(SCENE_MAIN_MENU, scn);
}

/*
This method loads the main menu scene.
*/
static void scene_main_menu_load() {
    d("[scene_main_menu_load]: stub\n");
    d_scene_init(__FILE__);
}

static bool raw_selected = false;

/*
This method updates the main menu scene.
*/
static void scene_main_menu_update(int scene_frame) {
    raw_selected = mouse_in(
        0.25, 0.25, VIEWPORT_WIDTH - 0.5, VIEWPORT_HEIGHT - 0.5);
}

/*
This method draws the main menu scene.
*/
static void scene_main_menu_draw(int scene_frame, int scene_time_delta) {
    if (raw_selected) {
        glColor4f(1, 0, 0, 1);
    }

    double x = 0.25, y = 0.25;
    double width = VIEWPORT_WIDTH - 0.5, height = VIEWPORT_HEIGHT - 0.5;
    glBegin(GL_QUADS);
    glVertex2d(x, y);
    glVertex2d(x, y + height);
    glVertex2d(x + width, y + height);
    glVertex2d(x + width, y);
    glEnd();

    glColor4f(1, 1, 1, 1);
}

/*
This method unloads the main menu scene.
*/
static void scene_main_menu_unload() {
    d("[scene_main_menu_unload]: stub\n");
}
