#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#define PROJECT_NAME "PROJECT"
#define SCREEN_WIDTH 800.0
#define SCREEN_HEIGHT 600.0
#define REAL_WIDTH 1.0
#define REAL_HEIGHT 1.0
#define VIEWPORT_WIDTH (1 * (SCREEN_WIDTH/SCREEN_HEIGHT))
#define VIEWPORT_HEIGHT 1.0

#define SPRINTF_BUFFER_SIZE 4096


// ENGINE


#define ENGINE_VSYNC true
#define ENGINE_FPS 60.0
#define ENGINE_TICK 60.0


// scenes


#define SCENES_MAX 256
#define SCENE_MAIN_MENU 1

#endif  // SRC_CONFIG_H_
