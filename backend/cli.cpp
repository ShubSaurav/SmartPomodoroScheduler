#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include <ctime>
#include "Task.h"
#include "Scheduler.h"
#include "PomodoroTimer.h"

void printMenu() {
    std::cout << "\n\033[1;36m=== Smart Pomodoro Scheduler CLI ===\033[0m\n"
              << "1. Add Task\n"
              << "2. List Tasks\n"
              << "3. Optimize Schedule\n"
              << "4. Start Pomodoro Timer\n"
              << "5. Save Tasks\n"
              << "6. Load Tasks\n"
              << "7. Delete Task\n"
              << "8. Mark Task as Complete\n"
              << "9. View Task Statistics\n"
              << "0. Exit\n"
              << "Choose an option: ";
}

void addTask(Scheduler& scheduler) {
    static int nextId = 1;  // Simple ID generation
    std::string name;
    int duration, priority;
    
    std::cout << "\n\033[1;32m=== Add New Task ===\033[0m\n";
    std::cout << "Enter task name: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    
    std::cout << "Enter duration (minutes): ";
    std::cin >> duration;
    
    std::cout << "Enter priority (0:Critical, 1:High, 2:Medium, 3:Low): ";
    std::cin >> priority;
    
    std::tm deadline = {};
    std::cout << "Enter deadline (YYYY MM DD HH MM): ";
    std::cin >> deadline.tm_year >> deadline.tm_mon >> deadline.tm_mday 
             >> deadline.tm_hour >> deadline.tm_min;
    deadline.tm_year -= 1900;  // Adjust year
    deadline.tm_mon -= 1;      // Adjust month (0-11)
    deadline.tm_sec = 0;
    
    std::time_t t = std::mktime(&deadline);
    auto deadline_tp = std::chrono::system_clock::from_time_t(t);
    
    Task task(nextId++, name, duration, priority, deadline_tp, false, false);
    scheduler.addTask(task);
    
    std::cout << "\033[1;32mTask added successfully!\033[0m\n";
}

void listTasks(const Scheduler& scheduler) {
    std::cout << "\n\033[1;33m=== Task List ===\033[0m\n";
    std::cout << std::setw(30) << "Name"
              << std::setw(15) << "Duration"
              << std::setw(15) << "Priority"
              << std::setw(25) << "Deadline"
              << std::setw(15) << "Score" << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    
    for (const auto& task : scheduler.getOptimizedTasks()) {
        auto deadline_tm = task.getDeadline();
        std::cout << std::setw(30) << task.getName()
                  << std::setw(15) << task.getDuration()
                  << std::setw(15) << task.getPriority()
                  << std::setw(25) << std::put_time(&deadline_tm, "%Y-%m-%d %H:%M")
                  << std::setw(15) << task.getUrgencyScore() << std::endl;
    }
}

void runPomodoro(PomodoroTimer& timer) {
    std::cout << "\n\033[1;35m=== Pomodoro Timer ===\033[0m\n";
    std::cout << "1. Start 25/5/15 Session\n";
    std::cout << "2. Start 45/15/30 Session\n";
    std::cout << "3. Start Custom Session\n";
    std::cout << "Choose option: ";
    
    int choice;
    std::cin >> choice;
    
    int workMins = 25, shortBreakMins = 5, longBreakMins = 15;
    
    switch (choice) {
        case 1:
            // Default values already set
            break;
        case 2:
            workMins = 45;
            shortBreakMins = 15;
            longBreakMins = 30;
            break;
        case 3:
            std::cout << "Enter work duration (minutes): ";
            std::cin >> workMins;
            std::cout << "Enter short break duration (minutes): ";
            std::cin >> shortBreakMins;
            std::cout << "Enter long break duration (minutes): ";
            std::cin >> longBreakMins;
            break;
        default:
            std::cout << "Invalid choice\n";
            return;
    }
    
    timer.startSession(workMins, shortBreakMins, longBreakMins);
    std::cout << "\nPomodoro session started.\n";
    std::cout << "Work: " << workMins << "min, Short Break: " << shortBreakMins 
              << "min, Long Break: " << longBreakMins << "min\n";
    std::cout << "Press Enter to stop...";
    std::cin.ignore();
    std::cin.get();
}

int main() {
    Scheduler scheduler;
    PomodoroTimer timer;
    int choice;
    
    while (true) {
        printMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                addTask(scheduler);
                break;
            case 2:
                listTasks(scheduler);
                break;
            case 3: {
                int totalTime;
                std::cout << "Enter total available time (minutes): ";
                std::cin >> totalTime;
                scheduler.optimizeSchedule(totalTime);
                std::cout << "\033[1;32mSchedule optimized!\033[0m\n";
                listTasks(scheduler);
                break;
            }
            case 4:
                runPomodoro(timer);
                break;
            case 5:
                scheduler.saveSchedule("../data/schedule.txt");
                std::cout << "\033[1;32mTasks saved to schedule.txt\033[0m\n";
                break;
            case 6:
                scheduler.loadTasks("../data/task.txt");
                std::cout << "\033[1;32mTasks loaded from task.txt\033[0m\n";
                break;
            case 7: {
                int taskId;
                std::cout << "Enter task ID to delete: ";
                std::cin >> taskId;
                // Note: Delete functionality needs to be added to Scheduler
                std::cout << "Delete functionality coming soon!\n";
                break;
            }
            case 8: {
                int taskId;
                std::cout << "Enter task ID to mark as complete: ";
                std::cin >> taskId;
                // Note: Complete functionality needs to be added to Scheduler
                std::cout << "Complete functionality coming soon!\n";
                break;
            }
            case 9:
                // Add statistics view here
                std::cout << "Statistics view coming soon!\n";
                break;
            case 0:
                std::cout << "\033[1;36mGoodbye!\033[0m\n";
                return 0;
            default:
                std::cout << "\033[1;31mInvalid choice!\033[0m\n";
        }
    }
    
    return 0;
}