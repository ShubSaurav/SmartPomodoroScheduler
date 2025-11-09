#ifndef POMODORO_TIMER_H
#define POMODORO_TIMER_H

#include <vector>
#include "Task.h"

class PomodoroTimer {
public:
    // Overload for demo mode (integer-based)
    void startSession(int workMins, int shortBreakMins, int longBreakMins);

    // Overload for task-based mode
    void startSession(const std::vector<Task>& tasks);
};

#endif
