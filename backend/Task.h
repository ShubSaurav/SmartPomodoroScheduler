#pragma once
#include <string>
#include <ctime>
#include <iostream>
#include <chrono>
#include <memory>
#include <vector>
#include <queue>
#include <functional>
#include "JSON.h"

class Task; // Forward declaration

// AVL Tree Node for task organization
struct TaskNode {
    std::shared_ptr<Task> task;
    int height;
    std::shared_ptr<TaskNode> left;
    std::shared_ptr<TaskNode> right;
    
    TaskNode(std::shared_ptr<Task> t) : task(t), height(1), left(nullptr), right(nullptr) {}
};

class Task {
private:
    int id;
    std::string name;
    int duration;      // in minutes
    int priority;      // 0: Critical, 1: High, 2: Medium, 3: Low
    std::chrono::system_clock::time_point deadline;
    bool completed;
    bool active;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point completedAt;

public:
    // Constructor with modern C++ time handling
    Task(int id, std::string n = "", int d = 0, int p = 1, 
         std::chrono::system_clock::time_point dl = std::chrono::system_clock::now(),
         bool c = false, bool a = false);

    // JSON Methods
    JSON toJSON() const;
    static Task fromJSON(const JSON& json);

    // Getters
    std::string getName() const;
    int getDuration() const;
    int getPriority() const;
    std::tm getDeadline() const;
    bool isCompleted() const;

    // Setters
    void setName(std::string n);
    void setDuration(int d);
    void setPriority(int p);
    void setDeadline(std::tm dl);
    void setCompleted(bool c);

    // Utility
    int getUrgencyScore() const;
    void displayTask() const;
    std::string serialize() const;
    static Task deserialize(const std::string& line);
};
