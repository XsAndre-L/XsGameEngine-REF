#pragma once
// #include "CommonValues.h"
// #include <GLEW/glew.h>
#include <codeframe>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Window/GLFW_Window.h"

#include "../../AssetLoading/OpenGL_Assets/OpenGL_Assets.h"
#include "../../Time.h"
#include "../GUI_Renderer.h"
#include "Shaders.h"

#include "../../Camera.h"

class OpenGL_Renderer {
  public:
    OpenGL_Renderer() = default;
    ~OpenGL_Renderer() = default;

    int init_OpenGL_Renderer(GLFWwindow *newWindow); // initialize

    // void pushModel(const char* fileName);
    void draw();

    Shaders *getShaders() { return &shaders; }
    glm::mat4 *getViewMatrix() { return shaders.ViewMatrix(); }

    void shutdown_Renderer();

  private:
    GLFWwindow *window;

    Shaders shaders;

  public:
    OpenGL_Assets AssetManager;

    // OpenGL_GUI_Renderer GUI_Renderer;
    // std::vector<OpenGL_MeshModel*> modelList;
#ifdef OPENGL
    GUI_Renderer new_GUI_Renderer;
#endif
};
