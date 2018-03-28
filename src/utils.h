#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

void bezier_curve(double p0x, double p0y,
    double p1x, double p1y,
    double p2x, double p2y,
    double p3x, double p3y,
    double delta,
    double *out_x, double *out_y);
bool key_pressed(int key);
bool mouse_in(double x, double y, double w, double h);
bool mouse_pressed(bool left_button = false,
    bool middle_button = false,
    bool right_button = false);
void d_scene_init(const char *file);
void d_scene_update(int scene_frame);
void d_scene(int scene_frame, int scene_time_delta);

#endif  // SRC_UTILS_H_
