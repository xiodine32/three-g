#include "scene.h"
#include "utils.h"

static const scene_t *current_scene = NULL;
static int current_scene_id = -1;
static scene_t scenes[SCENES_MAX] = {0};

static int current_scene_draw_frame;
static int current_scene_update_frame;

static int current_scene_draw_last_tick;

static int current_scene_draw_delta;
static int current_scene_update_delta;

static GLubyte animation_status = 0;
static GLuint animation_frames = 0;
static GLuint animation_total_frames = 0;
static const scene_t *animation_target;
static int animation_target_id;


static void scene_set_active(int scene_id, const scene_t &scene);
static void scene_unload();

/*
This method draws the current scene.
*/
void scene_draw() {
    if (current_scene == NULL) return;

    current_scene_draw_frame++;

    int tick = (int)(glfwGetTime() * 1000);
    current_scene_draw_delta = tick - current_scene_draw_last_tick;
    current_scene_draw_last_tick = tick;

    d_scene(current_scene_draw_frame, current_scene_draw_delta);

    if (animation_status == 0) {
        current_scene->draw(current_scene_draw_frame,
            current_scene_draw_delta);
    }
    if (animation_status != 0) {
        double delta = animation_frames / (double)animation_total_frames;

        double bezier_delta;
        bezier_curve(BEZIER_PRETTY, delta, NULL, &bezier_delta);
        double inverted = animation_status == 1 ? 1 : -1;

        double end_delta = inverted * (1 - bezier_delta);
        double start_delta = inverted * (-1 * bezier_delta);

        glPushMatrix();

        glRotated(30 * end_delta, 0, 0, 1);
        glTranslated(0, end_delta, 0);

        animation_target->draw(-1, current_scene_draw_delta);

        glPopMatrix();

        glPushMatrix();

        glRotated(30 * start_delta, 0, 0, 1);
        glTranslated(0, start_delta, 0);

        current_scene->draw(current_scene_draw_frame,
            current_scene_draw_delta);

        glPopMatrix();
    }
}

/*
This method updates the current scene.
*/
void scene_update() {
    if (current_scene == NULL) return;

    current_scene_update_frame++;

    if (animation_status == 0) {
        d_scene_update(current_scene_update_frame);
        current_scene->update(current_scene_update_frame);
    }

    if (animation_status != 0) {
        animation_frames += (GLuint) ENGINE_TICK_RATE;
        if (animation_frames > animation_total_frames) {
            animation_status = 0;
            scene_unload();
            scene_set_active(animation_target_id, *animation_target);
            animation_target = NULL;
            current_scene->update(current_scene_update_frame);
        }
    }
}

/*
This method creates a new scene and stores it for later use.
*/
void scene_new(int scene_id, const scene_t &scene) {
    if (scene_id >= 0 && scene_id < SCENES_MAX) {
        scenes[scene_id] = scene;
        d("[scene_new]: Created scene %d\n", scene_id);
    } else {
        d("[scene_new]: Could not create scene %d\n", scene_id);
    }
}

/*
This method validates the scene to be set.
*/
static bool scene_valid(const scene_t &scene) {
    if (scene.load == NULL || scene.draw == NULL ||
        scene.update == NULL || scene.unload == NULL) {
        d("[scene_valid]: invalid pointers scene");
        d(" (load:%p draw:%p update:%p unload:%p)\n",
            scene.load, scene.draw,
            scene.update, scene.unload);
        return false;
    }

    return true;
}

static void scene_unload() {
    // unload active scene
    if (current_scene != NULL) {
        current_scene->unload();

        current_scene = NULL;
        current_scene_id = -1;
    }
}

static void scene_set_active(int scene_id, const scene_t &scene) {
    current_scene = &scene;
    current_scene_id = scene_id;

    current_scene_draw_frame = 0;
    current_scene_update_frame = 0;
    current_scene_draw_delta = 0;
    current_scene_update_delta = 0;

    current_scene->load();
}

/*
This method sets the current scene to the specified id.
If id is -1, there will not be a scene.
It also handles loading / unloading of scene resources.
*/
void scene_set(int scene_id) {
    scene_unload();

    // invalid id parsed?
    if (scene_id == -1) {
        d("[scene_set]: goodbye\n");
        return;
    }

    // find new scene
    const scene_t &possible_scene = scenes[scene_id];

    if (!scene_valid(possible_scene)) {
        d("[scene_set]: invalid scene %d\n", scene_id);
        return;
    }

    // set active scene
    scene_set_active(scene_id, possible_scene);
}

static void scene_animate(int scene_id, GLuint time_interval) {
    // invalid id parsed?
    if (scene_id == -1) {
        d("[scene_animate]: goodbye\n");
        return;
    }

    // find new scene
    const scene_t &possible_animation_scene = scenes[scene_id];

    // check for valid scene
    if (!scene_valid(possible_animation_scene)) {
        d("[scene_animate]: invalid scene %d\n", scene_id);
        return;
    }

    if (animation_status != 0) {
        d("[scene_animate]: overriding animation!\n");
    }

    animation_target_id = scene_id;
    animation_target = &possible_animation_scene;

    // update animation_status
    animation_frames = 0;

    animation_total_frames = time_interval;
}

/*
This method animates in the scene_id by using glTranslate and pushing colors.
*/
void scene_animate_in(int scene_id, GLuint time_interval) {
    scene_animate(scene_id, time_interval);
    animation_status = 1;
}

/*
This method animates in the scene_id by using glTranslate and pushing colors.
*/
void scene_animate_out(int scene_id, GLuint time_interval) {
    scene_animate(scene_id, time_interval);
    animation_status = 2;
}


/*
This method gets the current scene id.
*/
int scene_get_id() {
    return current_scene_id;
}
