#include "HTTPServer.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <cstring>

void RequestQueue::push(int client_socket, const HTTPRequest& request) {
    std::unique_lock<std::mutex> lock(mutex);
    requests.push({client_socket, request});
    condition.notify_one();
}

bool RequestQueue::pop(std::pair<int, HTTPRequest>& item) {
    std::unique_lock<std::mutex> lock(mutex);
    if (requests.empty()) {
        condition.wait_for(lock, std::chrono::seconds(1));
        if (requests.empty()) return false;
    }
    item = requests.front();
    requests.pop();
    return true;
}

bool RequestQueue::empty() {
    std::unique_lock<std::mutex> lock(mutex);
    return requests.empty();
}

HTTPServer::HTTPServer(int p) : port(p), running(false) {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    // Allow socket reuse
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Failed to bind socket");
    }
}

HTTPServer::~HTTPServer() {
    stop();
    close(server_socket);
}

void HTTPServer::addRoute(const std::string& path, RequestHandler handler) {
    handlers[path] = std::move(handler);
}

void HTTPServer::start() {
    running = true;
    listen(server_socket, SOMAXCONN);
    
    // Start worker threads
    for (int i = 0; i < NUM_THREADS; i++) {
        worker_threads.emplace_back([this] { workerThread(); });
    }

    std::cout << "Server started on port " << port << std::endl;

    while (running) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        
        if (client_socket < 0) {
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }

        // Read the request
        char buffer[4096] = {0};
        ssize_t bytes_read = read(client_socket, buffer, 4095);
        if (bytes_read > 0) {
            HTTPRequest request = parseRequest(buffer);
            request_queue.push(client_socket, request);
        } else {
            close(client_socket);
        }
    }
}

void HTTPServer::workerThread() {
    while (running) {
        std::pair<int, HTTPRequest> item;
        if (request_queue.pop(item)) {
            int client_socket = item.first;
            const HTTPRequest& request = item.second;

            // Find and execute handler
            HTTPResponse response(404, "{\"error\": \"Not Found\"}");
            auto it = handlers.find(request.path);
            if (it != handlers.end()) {
                try {
                    response = it->second(request);
                } catch (const std::exception& e) {
                    response = HTTPResponse(500, "{\"error\": \"Internal Server Error\"}");
                }
            }

            // Send response
            std::string response_str = serializeResponse(response);
            send(client_socket, response_str.c_str(), response_str.length(), 0);
            close(client_socket);
        }
    }
}

HTTPRequest HTTPServer::parseRequest(const std::string& raw_request) {
    HTTPRequest request;
    std::istringstream stream(raw_request);
    std::string line;

    // Parse request line
    std::getline(stream, line);
    std::istringstream request_line(line);
    request_line >> request.method >> request.path;

    // Parse headers
    while (std::getline(stream, line) && line != "\r") {
        auto delimiter_pos = line.find(':');
        if (delimiter_pos != std::string::npos) {
            std::string key = line.substr(0, delimiter_pos);
            std::string value = line.substr(delimiter_pos + 2);
            if (!value.empty() && value.back() == '\r') {
                value.pop_back();
            }
            request.headers[key] = value;
        }
    }

    // Parse body
    if (request.headers.count("Content-Length")) {
        int content_length = std::stoi(request.headers["Content-Length"]);
        std::vector<char> body_buffer(content_length);
        stream.read(body_buffer.data(), content_length);
        request.body = std::string(body_buffer.begin(), body_buffer.end());
    }

    return request;
}

std::string HTTPServer::serializeResponse(const HTTPResponse& response) {
    std::ostringstream stream;
    stream << "HTTP/1.1 " << response.status << " ";
    
    switch (response.status) {
        case 200: stream << "OK"; break;
        case 404: stream << "Not Found"; break;
        case 500: stream << "Internal Server Error"; break;
        default: stream << "Unknown"; break;
    }
    stream << "\r\n";

    for (const auto& [key, value] : response.headers) {
        stream << key << ": " << value << "\r\n";
    }
    stream << "Content-Length: " << response.body.length() << "\r\n";
    stream << "\r\n";
    stream << response.body;

    return stream.str();
}

void HTTPServer::stop() {
    running = false;
    for (auto& thread : worker_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    worker_threads.clear();
}