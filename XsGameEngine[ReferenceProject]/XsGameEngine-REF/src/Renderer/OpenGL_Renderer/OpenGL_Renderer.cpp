#include "OpenGL_Renderer.h"

int OpenGL_Renderer::init_OpenGL_Renderer(GLFWwindow* newWindow)
{
	window = newWindow;

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Glew initialization failed!");
		glfwDestroyWindow(newWindow);
		return false;

	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_KHR_vulkan_glsl);
	glEnable(GL_ARB_gl_spirv);	//Allows for spir-v Shaders

	int width, height;
	glfwGetWindowSize(newWindow, &width, &height);

	glViewport(0, 0, width, height);

	

	return 0;
}

void OpenGL_Renderer::shutdown_Renderer()
{
	printf("Deleting Window");
	glfwDestroyWindow(window);

	glfwTerminate();
}
