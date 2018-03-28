#include "all.h"
#include "engine.h"
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

bool mouse_in(double x, double y, double w, double h) {
    double xpos, ypos;
    glfwGetCursorPos(glfw_window, &xpos, &ypos);
    xpos /= SCREEN_WIDTH;
    ypos /= SCREEN_HEIGHT;
    if (xpos < 0 || ypos < 0) {
        return false;
    }
    if (xpos > 1 || ypos > 1) {
        return false;
    }
    xpos *= VIEWPORT_WIDTH;
    ypos *= VIEWPORT_HEIGHT;

    return (x < xpos && xpos < x + w) && (y < ypos && ypos < y + h);
}

bool key_pressed(int key) {
    return key_state[key];
}

bool mouse_pressed(bool left_button,
    bool middle_button,
    bool right_button) {
    left_button = left_button &&
        glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_LEFT)
            == GLFW_PRESS;
    middle_button = middle_button &&
        glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_MIDDLE)
            == GLFW_PRESS;
    right_button = right_button &&
        glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_RIGHT)
            == GLFW_PRESS;

    return left_button || middle_button || right_button;
}


void bezier_curve(double p0x, double p0y,
    double p1x, double p1y,
    double p2x, double p2y,
    double p3x, double p3y,
    double delta,
    double *out_x, double *out_y) {
    double powi2 = pow(1 - delta, 2);
    double powi3 = pow(1 - delta, 3);
    double pow2 = pow(delta, 2);
    double pow3 = pow(delta, 3);

    if (out_x != NULL) {
    *out_x = powi3 * p0x + 3 * delta * powi2 * p1x +
        3 * pow2 * (1 - delta) * p2x + pow3 * p3x;
    }

    if (out_y != NULL) {
    *out_y = powi3 * p0y + 3 * delta * powi2 * p1y +
        3 * pow2 * (1 - delta) * p2y + pow3 * p3y;
    }
}
