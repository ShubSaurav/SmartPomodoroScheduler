#include "HTTPServer.h"
#include "Scheduler.h"
#include "Task.h"
#include <fstream>
#include <sstream>
#include <iostream>

int main() {
    HTTPServer server(8080);
    Scheduler scheduler;
    
    // Root check
    server.addRoute("/", [](const HTTPRequest& req) -> HTTPResponse {
        if (req.method != "GET") return HTTPResponse(405);
        return HTTPResponse(200, "{\"status\": \"Smart Pomodoro Scheduler Backend Running!\"}");
    });

    // Get all tasks
    server.addRoute("/tasks", [](const HTTPRequest& req) -> HTTPResponse {
        if (req.method != "GET") return HTTPResponse(405);
        
        std::ifstream file("../data/task.txt");
        if (!file.is_open()) {
            return HTTPResponse(404, "{\"error\": \"Task file not found.\"}");
        }
        
        std::ostringstream output;
        output << "{\"tasks\": [";
        std::string line;
        bool first = true;
        while (std::getline(file, line)) {
            if (!first) output << ",";
            output << "\"" << line << "\"";
            first = false;
        }
        output << "]}";
        return HTTPResponse(200, output.str());
    });

    // Optimize schedule
    server.addRoute("/optimize", [](const HTTPRequest& req) -> HTTPResponse {
        if (req.method != "POST") return HTTPResponse(405);
        
        Scheduler scheduler;
        scheduler.loadTasks("../data/task.txt");
        scheduler.optimizeSchedule(120);
        scheduler.saveSchedule("../data/schedule.txt");
        return HTTPResponse(200, "{\"message\": \"Schedule optimized and saved!\"}");
    });

    std::cout << "Starting server on port 8080..." << std::endl;
    server.start();
    return 0;
}
