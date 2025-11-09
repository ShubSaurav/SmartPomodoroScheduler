#include "Task.h"
#include <iomanip>
#include <sstream>
#include <cmath>
#include <ctime>

// Constructor
Task::Task(int id, std::string n, int d, int p, 
           std::chrono::system_clock::time_point dl,
           bool c, bool a)
    : id(id), name(n), duration(d), priority(p), deadline(dl), 
      completed(c), active(a),
      createdAt(std::chrono::system_clock::now()),
      completedAt(std::chrono::system_clock::now()) {}

// =================== GETTERS ===================
std::string Task::getName() const { return name; }
int Task::getDuration() const { return duration; }
int Task::getPriority() const { return priority; }
std::tm Task::getDeadline() const {
    std::time_t tt = std::chrono::system_clock::to_time_t(deadline);
    return *std::localtime(&tt);
}
bool Task::isCompleted() const { return completed; }

// =================== SETTERS ===================
void Task::setName(std::string n) { name = n; }
void Task::setDuration(int d) { duration = d; }
void Task::setPriority(int p) { priority = p; }
void Task::setDeadline(std::tm dl) {
    std::time_t tt = std::mktime(&dl);
    deadline = std::chrono::system_clock::from_time_t(tt);
}
void Task::setCompleted(bool c) { 
    completed = c;
    if (c) {
        completedAt = std::chrono::system_clock::now();
    }
}

// =================== LOGIC ===================
// Calculates how urgent the task is based on priority, deadline proximity, and duration
int Task::getUrgencyScore() const {
    auto now = std::chrono::system_clock::now();
    auto deadline_time = std::chrono::system_clock::to_time_t(deadline);
    auto current_time = std::chrono::system_clock::to_time_t(now);
    
    // Time until deadline in hours
    double hours_left = std::difftime(deadline_time, current_time) / 3600.0;
    
    // Priority weights (reverse order because 0 is Critical)
    const int priority_weights[] = {100, 75, 50, 25}; // Critical, High, Medium, Low
    int priority_score = priority_weights[priority];
    
    // Deadline factor (increases score as deadline approaches)
    double deadline_factor = std::max(1.0, 24.0 / std::max(1.0, hours_left));
    
    // Duration factor (shorter tasks get slight preference, but not too much)
    double duration_factor = 1.0 + (60.0 - std::min(60.0, (double)duration)) / 60.0;
    
    // Calculate final score
    int score = static_cast<int>(priority_score * deadline_factor * duration_factor);
    
    // Bonus for overdue tasks
    if (hours_left <= 0) {
        score *= 2;  // Double the urgency for overdue tasks
    }
    
    return score;
}

// Display formatted task info in console
void Task::displayTask() const {
    std::cout << "Task: " << name
              << " | Duration: " << duration << " min"
              << " | Priority: " << priority
              << " | Urgency: " << getUrgencyScore()
              << (completed ? " | Status: Done" : " | Status: Pending")
              << std::endl;
}

// =================== JSON ===================
JSON Task::toJSON() const {
    JSON obj = JSON::Object();
    obj["id"] = id;
    obj["name"] = name;
    obj["duration"] = duration;
    obj["priority"] = priority;
    obj["deadline"] = std::chrono::system_clock::to_time_t(deadline);
    obj["completed"] = completed;
    obj["active"] = active;
    obj["createdAt"] = std::chrono::system_clock::to_time_t(createdAt);
    obj["completedAt"] = std::chrono::system_clock::to_time_t(completedAt);
    return obj;
}

Task Task::fromJSON(const JSON& json) {
    Task task(
        json["id"],
        json["name"],
        json["duration"],
        json["priority"],
        std::chrono::system_clock::from_time_t(json["deadline"]),
        json["completed"],
        json["active"]
    );
    // Set timestamps
    task.createdAt = std::chrono::system_clock::from_time_t(json["createdAt"]);
    task.completedAt = std::chrono::system_clock::from_time_t(json["completedAt"]);
    return task;
}

// =================== FILE I/O ===================
// Convert Task object to CSV-style string for saving
std::string Task::serialize() const {
    return toJSON().dump();
}

// Read one line from file and create Task object
Task Task::deserialize(const std::string& line) {
    // TODO: Implement JSON parsing from string
    // For now, maintaining backward compatibility with CSV format
    std::istringstream in(line);
    std::string token;
    
    std::getline(in, token, ',');
    std::string name = token;

    std::getline(in, token, ',');
    int duration = std::stoi(token);

    std::getline(in, token, ',');
    int priority = std::stoi(token);

    std::getline(in, token, ',');
    std::tm deadline = {};
    std::istringstream timeStream(token);
    timeStream >> std::get_time(&deadline, "%Y-%m-%d %H:%M");

    std::getline(in, token, ',');
    bool completed = (token == "1" || token == "true");

    return Task(0, name, duration, priority, 
                std::chrono::system_clock::from_time_t(std::mktime(&deadline)),
                completed, false);
}
