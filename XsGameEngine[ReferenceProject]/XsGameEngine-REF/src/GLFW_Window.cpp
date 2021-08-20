#include "GLFW_Window.h"
#include <Windows.h>

GLFW_Window::GLFW_Window()
{
	#pragma region Console
	//::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	//::ShowWindow(::GetConsoleWindow(), SW_MINIMIZE); //Still visible in the task bar
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	//::IsWindowVisible(::GetConsoleWindow()) != FALSE;
	#pragma endregion

	mainWindow = nullptr;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

GLFW_Window::~GLFW_Window()
{
}

void GLFW_Window::GetMaxMonitorResolution(int* w, int* h, int* refresh)
{
	int count;
	const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

	int maxWidth = 0;
	int maxHeight = 0;
	int maxRefresh = 0;
	for (int i = 0; i < count; i++)
	{
		if (modes[i].width > maxWidth)
			maxWidth = modes[i].width;
		if (modes[i].height > maxHeight)
			maxHeight = modes[i].height;
		if (modes[i].refreshRate > maxRefresh)
			maxRefresh = modes[i].refreshRate;
	}

	printf("MaxWidth: %d \n MaxHeight: %d \n MaxRefreshRate: %d \n", maxWidth, maxHeight, maxRefresh);

	*w = maxWidth;
	*h = maxHeight;
	*refresh = maxRefresh;
}

bool GLFW_Window::initWindow(std::string wName = "Test Name", const int width = 1280, const int height = 720)
{
	//Initialise GLFW
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API );		//SET GLFW TO NOT WORK WITH OPENGL
	//glfwWindowHint(GLFW_REFRESH_RATE, GLFW_FALSE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);		//DISABLES AUTOMATIC MINIMIZING
	//glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);			//ENABLE WINDOW RESIZE ABILLITY


	if (mainWindow == NULL)
	{
		mainWindow = glfwCreateWindow(width, height, wName.c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	
	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwDestroyWindow(mainWindow);
		return false;
	}

	//mainWindow = glfwCreateWindow(width, height, wName.c_str(), NULL, NULL);
	glfwSetWindowAspectRatio(mainWindow, 16, 9);

	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

	int maxHeight;
	int maxWidth;
	int maxRefreshRate;
	
	GetMaxMonitorResolution(&maxHeight, &maxWidth, &maxRefreshRate);
	//glfwGetMonitorPhysicalSize(monitors[0], &maxWidth, &maxHeight);
	//std::cout << "Max Monitor Resolution: " << maxHeight << " : " << maxWidth << std::endl;
	//int maxHeight = glfwGetMonitor


	glfwSetWindowSizeLimits(mainWindow, width, height, maxWidth, maxHeight);

	createCallbacks();
	glfwSetCursorPos(mainWindow, 0, 0);
	//glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(mainWindow, this);		// Creates pointer on the window handle
	//glfwSetWindowUserPointer(window, this);
	//glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	if (mainWindow) {
		return true;
	}
	else {
		return false;
	}
}

void GLFW_Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, key_callback);
	glfwSetCursorPosCallback(mainWindow, cursor_position_callback);
	glfwSetMouseButtonCallback(mainWindow, mouse_button_callback);
	glfwSetScrollCallback(mainWindow, scroll_callback);
}


#pragma region Input Callbacks

GLfloat GLFW_Window::cursorXChange()
{
	GLfloat change = xChange;
	xChange = 0;
	return change;
}

GLfloat GLFW_Window::cursorYChange()
{
	GLfloat change = yChange;
	yChange = 0;
	return change;
}

// Keyboard input
void GLFW_Window::key_callback(GLFWwindow* window, int key, int code, int action, int mode)
{
	GLFW_Window* theWindow = static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window));

	// Exit Application Action [Hold Escape]
	if (key == GLFW_KEY_ESCAPE && action == GLFW_REPEAT)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// F11 to Toggle Fullscreen Mode
	if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {

		GLFWmonitor* currentMonitor = glfwGetWindowMonitor(theWindow->getWindow());
		int maxWidth;
		int maxHeight;
		int maxRefreshRate;
		theWindow->GetMaxMonitorResolution(&maxWidth, &maxHeight, &maxRefreshRate);

		if (currentMonitor == nullptr) {
			printf("MODE - FULLSCREEN");
			glfwSetWindowMonitor(theWindow->getWindow(), glfwGetPrimaryMonitor(),0,0 ,1920, 1080, maxRefreshRate);
		}
		else {
			printf("MODE - WINDOWED");
			
			glfwSetWindowMonitor(theWindow->getWindow(), nullptr, 0, 0, 1280, 720, maxRefreshRate);
			glfwSetWindowAspectRatio(theWindow->getWindow(), 16, 9);

			glfwRestoreWindow(theWindow->getWindow());
			glfwMaximizeWindow(theWindow->getWindow());
			
			glfwSetWindowSizeLimits(theWindow->getWindow(), 1280, 720, maxWidth, maxHeight);
		}
			
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

// cursor Position Callback
void GLFW_Window::cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	// Get Pointer to window because function is static
	GLFW_Window* theWindow = static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window));

	/*if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}*/
	if (theWindow->CursorNormal == false) {
		// Position change is = to diffrence between last position and current position
		theWindow->xChange = xPos - theWindow->lastX; 
		theWindow->yChange = theWindow->lastY - yPos;

		//glm::vec2 p = glm::clamp(glm::vec2(theWindow->xChange, theWindow->yChange), glm::vec2(-100.0f, -100.0f), glm::vec2(100.0f, 100.0f));

		//theWindow->xChange = p.x;
		//theWindow->yChange = p.y;
		// Set last position to current position
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
	}
	else if (theWindow->lastX != xPos) {
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
	}
	

}

void GLFW_Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	GLFW_Window* theWindow = static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window));

	//When Right Mouse Button is Down Cursor disapears and we can move around the screen
	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
	{
		//theWindow->SelectAction = false;
		
		//if (theWindow->SelectAction == true)
		//{
		//	GLbyte color[4];
		//	GLfloat depth;
		//	GLuint index;

		//	/*glReadPixels(xPos, theWindow->bufferHeight + yPos - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
		//	glReadPixels(xPos, theWindow->bufferHeight + yPos - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		//	glReadPixels(xPos, theWindow->bufferHeight + yPos - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);*/

		//	glReadPixels(theWindow->bufferWidth / 2, theWindow->bufferHeight / 2, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
		//	glReadPixels(theWindow->bufferWidth / 2, theWindow->bufferHeight / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		//	glReadPixels(theWindow->bufferWidth / 2, theWindow->bufferHeight / 2, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

		//	printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
		//		theWindow->bufferWidth / 2, theWindow->bufferHeight / 2, color[0], color[1], color[2], color[3], depth, index);

		//	const glm::vec4 viewport = glm::vec4(0, 0, theWindow->bufferWidth, theWindow->bufferHeight);
		//	const glm::vec3 wincoord = glm::vec3(theWindow->bufferWidth / 2, theWindow->bufferHeight / 2, depth);
		//	GLuint view = theWindow->shaders->getUniformView();
		//	//GLuint projection = theWindow->shaders->getUniformProjection();
		//	const glm::mat4 modelview = theWindow->shaders->GetViewMatrix();
		//	const glm::mat4 projection = glm::perspective(45.0f, (GLfloat)theWindow->bufferWidth / (GLfloat)theWindow->bufferHeight, 0.1f, 100.0f);

		//	glm::vec3 objcoord = glm::unProject(wincoord, modelview, projection, viewport);// wincoord, view, projection, viewport


		//	printf("Coordinates in object space: %f, %f, %f\n",
		//		objcoord.x, objcoord.y, objcoord.z);
		//}


		if (theWindow->CursorNormal == true)
		{
			
			printf("Disable Cursor\n");
			//-- Raw mouse motion
			//When the cursor is disabled, raw(unscaled and unaccelerated) mouse motion can be enabled if available.
			//Raw mouse motion is closer to the actual motion of the mouse across a surface.It is not affected by the 
			//scalingand acceleration applied to the motion of the desktop cursor.That processing is suitable for a cursor 
			//while raw motion is better for controlling for example a 3D camera.Because of this, raw mouse motion 
			//is only provided when the cursor is disabled.
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

			//Disable Cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			theWindow->CursorNormal = false;
			
		}

	}

	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_RELEASE)
	{
		if (theWindow->CursorNormal == false)
		{
			
			printf("Enable Cursor\n");
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			theWindow->CursorNormal = true;
			
		}
	}

}
// Scroll input
void GLFW_Window::scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	GLFW_Window* theWindow = static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->scrollPos + yOffset < 10.0f && theWindow->scrollPos + yOffset > 0.0f)
	{
		
		theWindow->scrollChanges = yOffset;
		theWindow->scrollPos += yOffset;
	}
}
#pragma endregion