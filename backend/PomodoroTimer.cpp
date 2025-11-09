#include "PomodoroTimer.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void PomodoroTimer::startSession(int workMins, int shortBreakMins, int longBreakMins) {
    cout << "\nStarting Pomodoro demo session..." << endl;

    cout << "Focus Task: Example Task | Duration: " << workMins << " min" << endl;
    this_thread::sleep_for(chrono::seconds(2)); // simulate work
    cout << "[Work] Done!" << endl;

    cout << "\nTake a short break (" << shortBreakMins << " min)" << endl;
    this_thread::sleep_for(chrono::seconds(1)); // simulate short break
    cout << "[Short Break] Done!" << endl;

    cout << "\n✨ Time for a long break (" << longBreakMins << " min) ✨" << endl;
    this_thread::sleep_for(chrono::seconds(1)); // simulate long break
    cout << "[Long Break] Done!" << endl;
}

void PomodoroTimer::startSession(const vector<Task>& tasks) {
    for (const auto& task : tasks) {
        cout << "\nFocus Task: " << task.getName()
             << " | Duration: " << task.getDuration() << " min"
             << " | Priority: " << task.getPriority() << endl;

        this_thread::sleep_for(chrono::seconds(1)); // simulate work
        cout << "[Work] Done!" << endl;

        cout << "\nTake a short break (5 min)" << endl;
        this_thread::sleep_for(chrono::seconds(1));
        cout << "[Short Break] Done!" << endl;
    }

    cout << "\n✅ All Pomodoro cycles completed successfully!" << endl;
}
