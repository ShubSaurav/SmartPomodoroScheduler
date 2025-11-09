#include "Scheduler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

bool Scheduler::loadTasks(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open " << filename << std::endl;
        return false;
    }

    tasks.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        tasks.push_back(Task::deserialize(line));
    }
    file.close();
    return true;
}

bool Scheduler::saveSchedule(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot write to " << filename << std::endl;
        return false;
    }

    for (const auto& t : optimizedTasks)
        file << t.serialize() << "\n";

    file.close();
    return true;
}

bool Scheduler::compareByUrgency(const Task& a, const Task& b) {
    return a.getUrgencyScore() > b.getUrgencyScore();
}

void Scheduler::greedyOptimization(int totalTime) {
    // Sort tasks by urgency score
    std::sort(tasks.begin(), tasks.end(), compareByUrgency);
    
    int usedTime = 0;
    for (const auto& task : tasks) {
        if (usedTime + task.getDuration() <= totalTime) {
            optimizedTasks.push_back(task);
            usedTime += task.getDuration();
        }
    }
}

void Scheduler::knapsackOptimization(int totalTime) {
    int n = tasks.size();
    
    // Create DP table
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(totalTime + 1, 0));
    std::vector<std::vector<bool>> selected(n + 1, std::vector<bool>(totalTime + 1, false));
    
    // Fill DP table (0-1 Knapsack)
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= totalTime; w++) {
            if (tasks[i-1].getDuration() <= w) {
                int val = tasks[i-1].getUrgencyScore();
                if (val + dp[i-1][w - tasks[i-1].getDuration()] > dp[i-1][w]) {
                    dp[i][w] = val + dp[i-1][w - tasks[i-1].getDuration()];
                    selected[i][w] = true;
                } else {
                    dp[i][w] = dp[i-1][w];
                }
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }
    
    // Reconstruct solution
    optimizedTasks.clear();
    int w = totalTime;
    for (int i = n; i > 0; i--) {
        if (selected[i][w]) {
            optimizedTasks.push_back(tasks[i-1]);
            w -= tasks[i-1].getDuration();
        }
    }
    
    // Sort optimized tasks by urgency (for better user experience)
    std::sort(optimizedTasks.begin(), optimizedTasks.end(), compareByUrgency);
}

void Scheduler::optimizeSchedule(int totalTime) {
    optimizedTasks.clear();
    
    if (tasks.empty()) {
        return;
    }
    
    // Use Knapsack for smaller sets of tasks (more optimal but O(n*W) complexity)
    // Use Greedy for larger sets (less optimal but O(n log n) complexity)
    if (tasks.size() <= 20) {
        knapsackOptimization(totalTime);
    } else {
        greedyOptimization(totalTime);
    }
}

void Scheduler::addTask(const Task& task) {
    tasks.push_back(task);
}

void Scheduler::clearTasks() {
    tasks.clear();
    optimizedTasks.clear();
}
