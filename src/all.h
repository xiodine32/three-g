#ifndef SRC_ALL_H_
#define SRC_ALL_H_


#ifdef DEBUG
#define d(...) printf(__VA_ARGS__);
#else
#define d(...)
#endif

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>

#define GLFW_DLL
#include <glfw/glfw3.h>
#include <png.h>

#include "config.h"

#endif  // SRC_ALL_H_
