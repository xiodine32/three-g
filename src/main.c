#include "all.h"
#include "engine.h"

int main(void) {
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // please no resize
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(
        SCREEN_WIDTH, SCREEN_HEIGHT,
        PROJECT_NAME,
        NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    if (ENGINE_VSYNC) {
        glfwSwapInterval(60.0 / ENGINE_FPS);
    }

    engine_load(window);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        engine_run();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    engine_close();

    glfwTerminate();
    return 0;
}
