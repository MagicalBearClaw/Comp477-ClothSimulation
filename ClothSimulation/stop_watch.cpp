#include "stdafx.h"
#include "stop_watch.h"

StopWatch::StopWatch(): _currentTimePoint(std::chrono::high_resolution_clock::now())
{
}

float StopWatch::ElapsedTime()
{
    const std::chrono::high_resolution_clock::time_point lastTimePoint = _currentTimePoint;
    _currentTimePoint = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<float> elapsedTime = _currentTimePoint - lastTimePoint;
    return elapsedTime.count();
}

float StopWatch::Peek()
{
    return std::chrono::duration<float>(std::chrono::high_resolution_clock::now() -_currentTimePoint).count();
}
