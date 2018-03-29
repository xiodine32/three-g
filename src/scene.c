#include "all.h"

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


#define SP_COUNT 128
#define SP_LIFE_MAX 2.0
#define SP_SIZE 0.02

static double sp_x[SP_COUNT];
static double sp_y[SP_COUNT];
static double sp_dx[SP_COUNT];
static double sp_dy[SP_COUNT];
static double sp_life[SP_COUNT];

// TODO(@xiodine32): Refactor out simple_particles.
// TODO(@xiodine32): Refactor out S R T animations.



static void simple_particles_update() {
    for (int i = 0; i < SP_COUNT; i++) {
        if (sp_life[i] <= 0) {
            // spawn
            sp_life[i] = SP_LIFE_MAX * (0.3 + myrand() / (double) 32767);
            sp_x[i] = -1 + 2 * VIEWPORT_WIDTH * (myrand() / (double) 32767);
            sp_y[i] = -1 + 2 * VIEWPORT_HEIGHT * (myrand() / (double) 32767);
            sp_dx[i] = (myrand() / (double) 32767 - 0.5);
            sp_dy[i] = (myrand() / (double) 32767 - 0.5);
        }
        sp_life[i] -= ENGINE_TICK_RATE / 1000.0;
        sp_x[i] += 0.002 * sp_dx[i];
        sp_y[i] += 0.002 * sp_dy[i];
    }
}

static void simple_particles_draw() {
    for (int i = 0; i < SP_COUNT; i++) {
        double delta = sp_life[i] / (2 * SP_LIFE_MAX);

        glBegin(GL_TRIANGLE_FAN);

        glColor4f(1, 1, 1, min(delta, 1 - delta));
        glVertex2d(sp_x[i], sp_y[i]);

        glColor4f(1, 1, 1, 0);
        for (double d = 0; d < 2 * 3.14159265; d += 0.04) {
            glVertex2d(sp_x[i] + SP_SIZE * cos(d), sp_y[i] + SP_SIZE * sin(d));
        }

        glVertex2d(sp_x[i] + SP_SIZE, sp_y[i]);

        glEnd();
    }
    glColor4f(1, 1, 1, 1);
}

/*
This method draws the current scene.
*/
void scene_draw() {
    if (current_scene == NULL) return;

    current_scene_draw_frame++;

    int tick = (int)(glfwGetTime() * 1000);
    current_scene_draw_delta = tick - current_scene_draw_last_tick;
    current_scene_draw_last_tick = tick;

    if (animation_status == 0) {
        glColor4f(0.235, 0.235, 0.235, 1);

        glBegin(GL_QUADS);

        glVertex2d(0, 0);
        glVertex2d(0, VIEWPORT_HEIGHT);
        glVertex2d(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
        glVertex2d(VIEWPORT_WIDTH, 0);

        glEnd();

        glColor4f(1, 1, 1, 1);

        d_scene(current_scene_draw_frame, current_scene_draw_delta);

        current_scene->draw(current_scene_draw_frame,
            current_scene_draw_delta);
    }

    if (animation_status != 0) {
        double delta = animation_frames / (double)animation_total_frames;

        double bezier_delta;
        bezier_curve(BEZIER_PRETTY, delta, NULL, &bezier_delta);
        double inverted = animation_status == 1 ? 1 : -1;

        double scale_out_delta = 1;
        if (bezier_delta < 0.5) {
            scale_out_delta = bezier_delta / 0.5;
        }
        double scale_out = 1 - scale_out_delta;

        double scale_in_delta = 0;
        if (bezier_delta > 0.5) {
            scale_in_delta = (bezier_delta - 0.5) / 0.5;
        }
        double scale_in = scale_in_delta;

        double end_delta = inverted * (1 - bezier_delta);
        double start_delta = inverted * (-1 * bezier_delta);

        simple_particles_draw();

        glPushMatrix();

        glTranslated(0, 1 * end_delta, 0);
        glScaled(scale_in, scale_in, scale_in);
        glTranslated(0.5, 0.5, 0);
        glRotated(90 * end_delta, 0, 0, 1);
        glRotated(90 - 90 * scale_in, 0, 1, 0);
        glTranslated(-0.5, -0.5, 0);

        glColor4f(0.235, 0.235, 0.235, 1);

        glBegin(GL_QUADS);

        glVertex2d(0, 0);
        glVertex2d(0, VIEWPORT_HEIGHT);
        glVertex2d(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
        glVertex2d(VIEWPORT_WIDTH, 0);

        glEnd();

        glColor4f(1, 1, 1, 1);

        d_scene(current_scene_draw_frame, current_scene_draw_delta);

        animation_target->draw(-1, current_scene_draw_delta);

        glPopMatrix();

        glPushMatrix();

        glTranslated(0, 1 * start_delta, 0);
        glScaled(scale_out, scale_out, scale_out);
        glTranslated(0.5, 0.5, 0);
        glRotated(90 * start_delta, 0, 0, 1);
        glRotated(90 - 90 * scale_out, 0, 1, 0);
        glTranslated(-0.5, -0.5, 0);

        glColor4f(0.235, 0.235, 0.235, 1);

        glBegin(GL_QUADS);

        glVertex2d(0, 0);
        glVertex2d(0, VIEWPORT_HEIGHT);
        glVertex2d(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
        glVertex2d(VIEWPORT_WIDTH, 0);

        glEnd();

        glColor4f(1, 1, 1, 1);

        d_scene(current_scene_draw_frame, current_scene_draw_delta);

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

    simple_particles_update();

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
