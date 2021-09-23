#include "ProgramWindow.h"


ProgramWindow::ProgramWindow()
{
	mainWindow = NULL;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}


bool ProgramWindow::createWindow(GLint Width, GLint Height)
{
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		return false;
	}

	//Set Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Forwards Compatabillity
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	if (mainWindow == NULL)
	{
		mainWindow = glfwCreateWindow(Width, Height, "Test Windo", NULL, NULL);
	}
	

	if (!mainWindow) {
		printf("GLFW window creation failed");
		glfwDestroyWindow(mainWindow);
		return false;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for glew to use
	glfwMakeContextCurrent(mainWindow);

	//Allow modern extention features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Glew initialization failed!");
		glfwDestroyWindow(mainWindow);
		return false;

	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_KHR_vulkan_glsl);
	glEnable(GL_ARB_gl_spirv);	//Allows for spir-v Shaders

	glViewport(0, 0, bufferWidth, bufferHeight);

	createCallbacks();
	glfwSetCursorPos(mainWindow, 0 , 0);
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(mainWindow, this);		// Creates pointer on the window handle

	return true;
}

#pragma region Input Code
void ProgramWindow::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, key_callback);
	glfwSetCursorPosCallback(mainWindow, cursor_position_callback);
	glfwSetMouseButtonCallback(mainWindow, mouse_button_callback);
	glfwSetScrollCallback(mainWindow, scroll_callback);
}

GLfloat ProgramWindow::getXChange()
{
	GLfloat change = xChange;
	xChange = 0;
	return change;
}

GLfloat ProgramWindow::getYChange()
{
	GLfloat change = yChange;
	yChange = 0;
	return change;
}

// Keyboard input
void ProgramWindow::key_callback(GLFWwindow* window, int key, int code, int action, int mode)
{
	ProgramWindow* theWindow = static_cast<ProgramWindow*>(glfwGetWindowUserPointer(window));

	// Release Cursor Action [Press Escape]
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(window, nullptr);
		theWindow->CursorNormal = true;
	}

	// Exit Application Action [Hold Escape]
	if (key == GLFW_KEY_ESCAPE && action == GLFW_REPEAT)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("Pressed: %d\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("Released: %d\n", key);
		}
	}
}

// Mouse input
void ProgramWindow::cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	// Get Pointer to window because function is static
	ProgramWindow* theWindow = static_cast<ProgramWindow*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	// Position change is = to diffrence between last position and current position
	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	// Set last position to current position
	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
	
	

}
void ProgramWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	ProgramWindow* theWindow = static_cast<ProgramWindow*>(glfwGetWindowUserPointer(window));

	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS )
	{
		theWindow->SelectAction = true;

		if (theWindow->SelectAction == true)
		{
			GLbyte color[4];
			GLfloat depth;
			GLuint index;

			/*glReadPixels(xPos, theWindow->bufferHeight + yPos - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
			glReadPixels(xPos, theWindow->bufferHeight + yPos - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
			glReadPixels(xPos, theWindow->bufferHeight + yPos - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);*/

			glReadPixels(theWindow->bufferWidth / 2, theWindow->bufferHeight / 2, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
			glReadPixels(theWindow->bufferWidth / 2, theWindow->bufferHeight / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
			glReadPixels(theWindow->bufferWidth / 2, theWindow->bufferHeight / 2, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

			printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
				theWindow->bufferWidth / 2, theWindow->bufferHeight / 2, color[0], color[1], color[2], color[3], depth, index);

			const glm::vec4 viewport = glm::vec4(0, 0, theWindow->bufferWidth, theWindow->bufferHeight);
			const glm::vec3 wincoord = glm::vec3(theWindow->bufferWidth / 2, theWindow->bufferHeight / 2, depth);
			GLuint view = theWindow->shaders->getUniformView();
			//GLuint projection = theWindow->shaders->getUniformProjection();
			const glm::mat4 modelview = *theWindow->shaders->GetViewMatrix();
			const glm::mat4 projection = glm::perspective(45.0f, (GLfloat)theWindow->bufferWidth / (GLfloat)theWindow->bufferHeight, 0.1f, 100.0f);

			glm::vec3 objcoord = glm::unProject(wincoord, modelview,projection, viewport );// wincoord, view, projection, viewport
			

			printf("Coordinates in object space: %f, %f, %f\n",
				objcoord.x, objcoord.y, objcoord.z);
		}

		
		if (theWindow->CursorNormal = true)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPosCallback(window, cursor_position_callback);
			theWindow->CursorNormal = false;
		}
		
	}

	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
	{
		theWindow->SelectAction = false;
	}

}
// Scroll input
void ProgramWindow::scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	ProgramWindow* theWindow = static_cast<ProgramWindow*>(glfwGetWindowUserPointer(window));

	if ( theWindow->scrollPos + yOffset < 10.0f && theWindow->scrollPos + yOffset > 0.0f)
	{
		theWindow->scrollChanges = yOffset;
		theWindow->scrollPos += yOffset;
	}
}
#pragma endregion



void ProgramWindow::destroyWindow()
{
	glfwDestroyWindow(mainWindow);
}

ProgramWindow::~ProgramWindow()
{
	destroyWindow();
}


