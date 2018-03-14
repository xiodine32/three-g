#ifndef __ALL_H
#define __ALL_H


#ifdef DEBUG
#define d(...) printf(__VA_ARGS__);
#else
#define d(...) ;
#endif

#include <stdio.h>
#include <math.h>
#include <stdarg.h>

#define GLFW_DLL
#include <glfw/glfw3.h>
#include <png.h>

#include "config.h"

#endif // __ALL_H 