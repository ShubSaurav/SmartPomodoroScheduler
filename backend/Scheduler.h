#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "Task.h"

class Scheduler {
private:
    std::vector<Task> tasks;
    std::vector<Task> optimizedTasks;
    
    // Helper functions for optimization
    void knapsackOptimization(int totalTime);
    void greedyOptimization(int totalTime);
    static bool compareByUrgency(const Task& a, const Task& b);

public:
    bool loadTasks(const std::string& filename);
    bool saveSchedule(const std::string& filename);
    void optimizeSchedule(int totalTime);
    std::vector<Task> getOptimizedTasks() const { return optimizedTasks; }
    void addTask(const Task& task);
    void clearTasks();
};

#endif
