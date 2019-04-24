#include "timer.h"

std::chrono::time_point<std::chrono::steady_clock> Timer::startTime;

void Timer::start() {
    startTime = std::chrono::steady_clock::now();
}

double Timer::takeTime() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - startTime).count() / 1e9;
}