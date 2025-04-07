#include "Timer.h"
#include <iostream>

Timer::Timer()
{
	
	isRunning = false;
}

void Timer::timeStart()
{
	if (!isRunning)
	{
		STime = cClock::now();
		isRunning = true;
	}
	cout << "Timer started" << endl;

}
void Timer::timeStop()
{
	if (isRunning)
	{
		ETime = cClock::now();
		isRunning = false;
	}
	cout << "Timer end" << endl;
}

double Timer::getElapsedTime()
{
	float currTime = 0.0f;
	chrono::milliseconds tElasped;
	if (isRunning)
	{
		tPoints currentTime = cClock::now();
		tElasped = chrono::duration_cast<chrono::milliseconds>(currentTime - STime);
		currTime = tElasped.count() / 1000.0f;
		return currTime;
	}
	else if (!isRunning)
	{
		tElasped = chrono::duration_cast<chrono::milliseconds>(ETime - STime);
		currTime = tElasped.count() / 1000.0f;
		return currTime;
	}
}