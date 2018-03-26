#ifndef SRC_ENGINE_H_
#define SRC_ENGINE_H_

#include "all.h"

extern bool key_state[512];
extern GLFWwindow *glfw_window;
void engine_load(GLFWwindow *window);
void engine_run();
void engine_close();

#endif  // SRC_ENGINE_H_
