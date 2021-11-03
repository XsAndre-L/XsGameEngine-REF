#pragma once
// Time class keeps track of everyting time related
// 
// keeps track of delta time inside loops -- Time.deltaTime();

#include <GLFW/glfw3.h>
#include <iostream>

class Time
{
public:
	Time();
	~Time();

	float getDeltaTime();
	//float timer();

	int getFps();

	void startTimeTracker();

private:
	bool timerActive = false;

	float lastTime = 0.0f;

	float deltaTime = 0.0f;
	float secondTracker = 0.0f;

	bool fpsEnabled = false;
	int fpsTracker = 0;
	int fps = 0;
};

