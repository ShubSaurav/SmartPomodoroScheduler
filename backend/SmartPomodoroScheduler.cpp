#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "Scheduler.h"
#include "Task.h"
#include "PomodoroTimer.h"
#include "Utils.h"

using namespace std;

// ======================================================
// FUNCTION: Process all tasks and save optimized schedule
// ======================================================
void processTasks() {
    Scheduler scheduler;

    cout << "===========================================" << endl;
    cout << "🧠 Smart Pomodoro Scheduler - File Version " << endl;
    cout << "===========================================" << endl;

    cout << "\nLoading tasks from data/task.txt..." << endl;
    if (!scheduler.loadTasks("data/task.txt")) {
        cerr << "❌ Error: Cannot open task file. Make sure it exists." << endl;
        return;
    }

    cout << "Optimizing schedule..." << endl;
    scheduler.optimizeSchedule(120);

    cout << "Saving optimized schedule to data/schedule.txt..." << endl;
    if (!scheduler.saveSchedule("data/schedule.txt")) {
        cerr << "❌ Error: Cannot save schedule file!" << endl;
        return;
    }

    cout << "\n✅ Schedule optimized and saved successfully!" << endl;
    cout << "===========================================" << endl;

    // Display optimized tasks
    cout << "\n===== SMART POMODORO SCHEDULE =====" << endl;
    for (const auto& t : scheduler.getOptimizedTasks()) {
        cout << "Task: " << t.getName()
             << " | Duration: " << t.getDuration() << " min"
             << " | Priority: " << t.getPriority() << endl;
    }
    cout << "===========================================" << endl;

    cout << "\n✅ Run complete. Results written to schedule.txt\n" << endl;
}

// ======================================================
// FUNCTION: Run a short Pomodoro demo session (DEFINITION)
// ======================================================
void startPomodoroDemo() {
    PomodoroTimer timer;
    cout << "\n===== STARTING SMART POMODORO SESSION =====" << endl;
    timer.startSession(1, 1, 2); // (work, short break, long break)
    cout << "\n✅ All Pomodoro cycles completed successfully!" << endl;
    cout << "=====================================================" << endl;
}

// ======================================================
// MAIN FUNCTION
// ======================================================
int main() {
    cout << "===========================================" << endl;
    cout << "SMART POMODORO SCHEDULER (C++ Backend)" << endl;
    cout << "===========================================" << endl;

    cout << "\nOptions:\n";
    cout << "1. Process and Optimize Tasks (from file)" << endl;
    cout << "2. Run Pomodoro Demo Session" << endl;
    cout << "3. Exit" << endl;
    cout << "-------------------------------------------" << endl;

    int choice;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            processTasks();
            break;
        case 2:
            startPomodoroDemo();   // ✅ now defined above main()
            break;
        case 3:
            cout << "👋 Exiting Smart Pomodoro Scheduler." << endl;
            return 0;
        default:
            cout << "❌ Invalid choice! Please select 1–3." << endl;
            break;
    }

    return 0;
}
