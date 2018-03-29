#include "scene.h"

#include "font.h"
#include "utils.h"

static void scene_main_menu_load();
static void scene_main_menu_update(int scene_frame);
static void scene_main_menu_draw(int scene_frame, int scene_time_delta);
static void scene_main_menu_unload();
static GLubyte button_about = 0;
static GLubyte button_play = 0;

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

    button_about = 0;
    button_play = 0;
}

/*
This method updates the main menu scene.
*/
static void scene_main_menu_update(int scene_frame) {
    button_about = mouse_in(0.25, 0.25,
        VIEWPORT_WIDTH - 1, VIEWPORT_HEIGHT - 0.5);
    button_about = button_about == 1 && mouse_pressed(true) ? 2 : button_about;

    button_play = mouse_in(VIEWPORT_WIDTH - 0.65, 0.25,
        VIEWPORT_WIDTH - 1, VIEWPORT_HEIGHT - 0.5);
    button_play = button_play == 1 && mouse_pressed(true) ? 2 : button_play;

    if (button_about == 2) {
        scene_animate_in(SCENE_ABOUT);
    }
}

/*
This method draws the main menu scene.
*/
static void scene_main_menu_draw(int scene_frame, int scene_time_delta) {
    double x, y, width, height;
    // TODO(@xiodine32): Fix this bloody viewport x/y coord issue.
    switch (button_about) {
        case 1:
            glColor4f(1, 0, 0, 0.4);
            break;
        case 2:
            glColor4f(0, 1, 0, 0.4);
            break;
        default:
            glColor4f(1, 1, 1, 0.25);
            break;
    }

    x = 0.25;
    y = 0.25;
    width = VIEWPORT_WIDTH - 1;
    height = VIEWPORT_HEIGHT - 0.5;

    glBegin(GL_QUADS);
    glVertex2d(x, y);
    glVertex2d(x, y + height);
    glVertex2d(x + width, y + height);
    glVertex2d(x + width, y);
    glEnd();

    switch (button_play) {
        case 1:
            glColor4f(1, 0, 0, 0.4);
            break;
        case 2:
            glColor4f(0, 1, 0, 0.4);
            break;
        default:
            glColor4f(1, 1, 1, 0.25);
            break;
    }

    x = VIEWPORT_WIDTH - 0.65;
    y = 0.25;
    width = VIEWPORT_WIDTH - 1;
    height = VIEWPORT_HEIGHT - 0.5;

    glBegin(GL_QUADS);
    glVertex2d(x, y);
    glVertex2d(x, y + height);
    glVertex2d(x + width, y + height);
    glVertex2d(x + width, y);
    glEnd();

    glColor4f(1, 1, 1, 1);
    font_draw_left(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2 - 16 * 2.5, 32,
        "About");
    font_draw_left(SCREEN_HEIGHT / 2, SCREEN_WIDTH - 16 * 2.5, 32,
        "Play");
}

/*
This method unloads the main menu scene.
*/
static void scene_main_menu_unload() {
    d("[scene_main_menu_unload]: stub\n");
    button_about = 0;
    button_play = 0;
}
