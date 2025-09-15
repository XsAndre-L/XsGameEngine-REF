#include "PreprocessorDefs.h"
#include "Renderer/GUI_Renderer.h"

#ifdef VULKAN
#include "Renderer/Vulkan_Renderer/Vulkan_Renderer.h"
Vulkan_Renderer Renderer;
#elif OPENGL
#include "Renderer/OpenGL_Renderer/OpenGL_Renderer.h"
OpenGL_Renderer Renderer;
#else
#endif

#include "Time.h"

Window newWindow;
Time programTime;
Camera camera;

/**
 * The Entry Point
 * @author Andre Loubser
 *
 */
int main() {
    // CREATE WINDOW
    if (!newWindow.initWindow("WIN", 1920, 1080)) { // 1280, 720
        return EXIT_FAILURE;
    }

#ifdef VULKAN
    // CREATE VULKAN INSTANCE -- needs the window so we get the window
    if (Renderer.init_Vulkan_Renderer(newWindow.getWindow()) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
#elif OPENGL
    // CREATE VULKAN INSTANCE -- needs the window so we get the window
    if (Renderer.init_OpenGL_Renderer(newWindow.getWindow()) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
#endif

    float deltaTime = 0.0f;

    // camera needs to be set as active to change the vulkan view matrix
    camera.setAsActive(Renderer.getViewMatrix());

    int counter = 0;

    // PROGRAM LOOP
    while (!newWindow.shouldClose()) {
        glfwPollEvents();

        programTime.startTimeTracker();

        deltaTime = programTime.getDeltaTime();
        programTime.getFps();

        camera.checkInputs(newWindow.getCursorMode(), newWindow.getKeys(), newWindow.getScroll(), deltaTime);
        camera.setCamera(newWindow.getCursorMode(), newWindow.cursorXChange(), newWindow.cursorYChange(), deltaTime);

        Renderer.draw();
    }

    // CLEAN UP
    Renderer.shutdown_Renderer();

    glfwDestroyWindow(newWindow.getWindow());
    glfwTerminate();
}
