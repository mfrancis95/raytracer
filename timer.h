#include <chrono>

struct Timer {

    static void start();
    static double takeTime();

private:

    static std::chrono::time_point<std::chrono::steady_clock> startTime;

};