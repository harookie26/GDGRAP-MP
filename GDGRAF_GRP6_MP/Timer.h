#pragma once
#include <chrono>
using namespace std;
class Timer
{

private:
	typedef chrono::steady_clock cClock;
	typedef chrono::time_point<cClock> tPoints;

	tPoints STime;
	tPoints ETime;
	bool isRunning;

public:
	Timer();
	void timeStart();
	void timeStop();
	double getElapsedTime();
	void reset();

};

