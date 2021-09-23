#include "Time.h"

Time::Time()
{
}

Time::~Time()
{
}

// DeltaTime
float Time::getDeltaTime()
{
	return deltaTime;
}

// FPS
int Time::getFps()
{
	fpsEnabled = true;
	return fps;
}

void Time::startTimeTracker()
{


	// Delta time
	float now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;

	// Frames per second
	if (secondTracker < 1.0f)
	{
		//mesure how much frames is drawn every second
		fpsTracker++;
		secondTracker += deltaTime;
	}
	else
	{
		// set fps = total frames drawn in the last second 
		fps = fpsTracker;
		if (fpsEnabled == true)
		{
			printf("FPS : %d", fps);
			fpsEnabled = false;
		}

		secondTracker = 0.0f;
		fpsTracker = 0;
	}


}
