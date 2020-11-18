#pragma once
#include "stdafx.h"


class StopWatch
{
public:
	StopWatch();
	float ElapsedTime();
	float Peek();
private:
	std::chrono::high_resolution_clock::time_point _currentTimePoint;
};