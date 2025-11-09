#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>

// HTTP Request structure
struct HTTPRequest {
    std::string method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

// HTTP Response structure
struct HTTPResponse {
    int status;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    
    HTTPResponse(int s = 200, const std::string& b = "") : status(s), body(b) {
        headers["Content-Type"] = "application/json";
        headers["Access-Control-Allow-Origin"] = "*";
        headers["Access-Control-Allow-Methods"] = "GET, POST, PUT, DELETE";
        headers["Access-Control-Allow-Headers"] = "Content-Type";
    }
};

// Thread-safe request queue
class RequestQueue {
private:
    std::queue<std::pair<int, HTTPRequest>> requests;
    std::mutex mutex;
    std::condition_variable condition;

public:
    void push(int client_socket, const HTTPRequest& request);
    bool pop(std::pair<int, HTTPRequest>& item);
    bool empty();
};

class HTTPServer {
public:
    using RequestHandler = std::function<HTTPResponse(const HTTPRequest&)>;

private:
    int server_socket;
    int port;
    bool running;
    std::unordered_map<std::string, RequestHandler> handlers;
    std::vector<std::thread> worker_threads;
    RequestQueue request_queue;
    const int NUM_THREADS = 4;

    void handleClient(int client_socket);
    void workerThread();
    HTTPRequest parseRequest(const std::string& raw_request);
    std::string serializeResponse(const HTTPResponse& response);

public:
    HTTPServer(int port = 3000);
    ~HTTPServer();

    void addRoute(const std::string& path, RequestHandler handler);
    void start();
    void stop();
};