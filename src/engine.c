#include "engine.h"
#include "font.h"
#include "timers.h"
#include "scene.h"
#include "scene_main_menu.h"
#include "scene_about.h"

bool key_state[512] = {0};

static GLuint png_texture_load(const char *file_name,
    int *width = NULL, int *height = NULL);

static struct res {
    const char *path;
    int sprite_width;
    int sprite_height;
    GLuint texture_id;
} resources[] = {
    {"res\\test.png", 64, 64, 0},
    {"res\\font.png", 64, 32, 0},
    {NULL, 0, 0, 0}
};

static void engine_tick();
static void engine_second_tick();
static void key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods);

static int total_ticks = 0;
static int last_fps = 0;

GLFWwindow *glfw_window = NULL;

void engine_load(GLFWwindow *window) {
    glfw_window = window;

    glfwSetKeyCallback(window, &key_callback);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glOrtho(0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    d("[textures] init\n");
    for (int i = 0; resources[i].path != NULL; i++) {
        d("[textures] load '%s'\n", resources[i].path);
        resources[i].texture_id = png_texture_load(resources[i].path);
        d("[textures] done '%s' - %u\n", resources[i].path,
            resources[i].texture_id);
    }

    font_init(resources[1].texture_id,
        resources[1].sprite_width,
        resources[1].sprite_height,
        512,
        512);

    timer_new(1, ENGINE_TICK, &engine_tick);
    timer_new(2, 1, &engine_second_tick);

    scene_init_main_menu();
    scene_init_about();
    scene_set(SCENE_MAIN_MENU);
}

static void key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods) {
    key_state[key] = action == GLFW_PRESS;
    d("[key_callback]: key: %d = %d (%s)\n", key,
        key_state[key], glfwGetKeyName(key, scancode));
}

static void engine_tick() {
    scene_update();
}
static void engine_second_tick() {
    last_fps = total_ticks;
    #ifdef DEBUG
        char title[128] = {0};
        snprintf(title, sizeof(title), "[DEBUG] %s FPS: %d SCENE: %d",
            PROJECT_NAME, last_fps, scene_get_id());
        glfwSetWindowTitle(glfw_window, title);
    #endif
    total_ticks = 0;
}

void engine_run() {
    total_ticks++;
    timer_run();

    scene_draw();
}

void engine_close() {
    scene_set(-1);
    timer_delete(1);
    timer_delete(2);
}

static GLuint png_texture_load(const char *file_name, int *width, int *height) {
    png_byte header[8];

    FILE *fp = fopen(file_name, "rb");
    if (fp == 0) {
        perror(file_name);
        return 0;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8)) {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
        NULL, NULL);
    if (!png_ptr) {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth,
        &color_type, NULL, NULL, NULL);

    if (width) { *width = temp_width; }
    if (height) { *height = temp_height; }

    if (bit_depth != 8) {
        fprintf(stderr, "%s: Unsupported bit depth %d.  Must be 8.\n",
            file_name, bit_depth);
        return 0;
    }

    GLint format;
    switch (color_type) {
    case PNG_COLOR_TYPE_RGB:
        format = GL_RGB;
        break;
    case PNG_COLOR_TYPE_RGB_ALPHA:
        format = GL_RGBA;
        break;
    default:
        fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name,
            color_type);
        return 0;
    }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte *image_data = (png_byte *)malloc(rowbytes * temp_height *
        sizeof(png_byte) + 15);

    if (image_data == NULL) {
        fprintf(stderr, "error: could not allocate memory for image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with
    // libpng
    png_byte ** row_pointers = (png_byte **)malloc(temp_height *
        sizeof(png_byte *));

    if (row_pointers == NULL) {
        fprintf(stderr, "error: could not allocate memory for row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of
    // image_data
    for (unsigned int i = 0; i < temp_height; i++) {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // Generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format,
        GL_UNSIGNED_BYTE, image_data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    return texture;
}
