#include "scene.h"
#include "utils.h"

static scene_t *current_scene = NULL;
static int current_scene_id = -1;
static scene_t scenes[SCENES_MAX] = {0};

static int current_scene_draw_frame;
static int current_scene_update_frame;

static int current_scene_draw_last_tick;

static int current_scene_draw_delta;
static int current_scene_update_delta;

/*
This method draws the current scene.
*/
void scene_draw()
{
    if (current_scene == NULL) return;

    current_scene_draw_frame++;

    int tick = (int)(glfwGetTime() * 1000);
    current_scene_draw_delta = tick - current_scene_draw_last_tick;
    current_scene_draw_last_tick = tick;

    d_scene(current_scene_draw_frame, current_scene_draw_delta);

    current_scene->draw(current_scene_draw_frame, current_scene_draw_delta);
}

/*
This method updates the current scene.
*/
void scene_update()
{
    if (current_scene == NULL) return;

    current_scene_update_frame++;

    d_scene_update(current_scene_update_frame);

    current_scene->update(current_scene_update_frame);
}

/*
This method creates a new scene and stores it for later use.
*/
void scene_new(int scene_id, scene_t &scene)
{
    if (scene_id >= 0 && scene_id < SCENES_MAX) {
        scenes[scene_id] = scene;
        d("[scene_new]: Created scene %d\n", scene_id);
    } else {
        d("[scene_new]: Could not create scene %d\n", scene_id);
    }
    
}

/*
This method sets the current scene to the specified id.
If id is -1, there will not be a scene.
It also handles loading / unloading of scene resources.
*/
void scene_set(int scene_id)
{
    // unload active scene
    if (current_scene != NULL) {
        current_scene->unload();

        current_scene = NULL;
        current_scene_id = -1;
    }

    // invalid id parsed?
    if (scene_id == -1) {
        d("[scene_set]: goodbye\n");
        return;
    }

    // find new scene
    scene_t *possible_scene = &(scenes[scene_id]);

    // check for valid scene
    if (possible_scene->load == NULL || 
        possible_scene->draw == NULL ||
        possible_scene->update == NULL ||
        possible_scene->unload == NULL) {
        d("[scene_set]: Possible scene %d does not have all pointers valid. (load:%p draw:%p update:%p unload:%p)\n",
            scene_id,
            possible_scene->load,
            possible_scene->draw,
            possible_scene->update,
            possible_scene->unload
        );
        return;
    }

    // set active scene
    current_scene = possible_scene;
    current_scene_id = scene_id;

    current_scene_draw_frame = 0;
    current_scene_update_frame = 0;
    current_scene_draw_delta = 0;
    current_scene_update_delta = 0;

    current_scene->load();

}

/*
This method gets the current scene id.
*/
int scene_get_id()
{
    return current_scene_id;
}