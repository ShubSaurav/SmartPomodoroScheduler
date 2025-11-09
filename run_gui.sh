#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}Smart Pomodoro Scheduler - GUI Version${NC}"

# Change to the backend directory
cd "$(dirname "$0")/backend"

# Build the backend server
echo -e "\n${BLUE}Building backend server...${NC}"
g++ -std=c++17 \
    server.cpp \
    HTTPServer.cpp \
    Task.cpp \
    Scheduler.cpp \
    PomodoroTimer.cpp \
    Utils.cpp \
    -I/usr/local/include \
    -L/usr/local/lib \
    -lpthread \
    -o SmartPomodoroScheduler

# Check if build was successful
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Backend built successfully!${NC}"
    
    # Start the backend server
    echo -e "\n${BLUE}Starting backend server...${NC}"
    ./SmartPomodoroScheduler &
    BACKEND_PID=$!
    
    # Wait a moment for the server to start
    sleep 2
    
    # Change to frontend directory
    cd ../frontend
    
    # Check if python3 is available
    if command -v python3 &> /dev/null; then
        echo -e "\n${BLUE}Starting frontend server...${NC}"
        python3 -m http.server 8080 &
        FRONTEND_PID=$!
        echo -e "${GREEN}Frontend server started at http://localhost:8080${NC}"
        
        # Open the browser (MacOS specific)
        if [[ "$OSTYPE" == "darwin"* ]]; then
            open http://localhost:8080
        fi
    else
        echo -e "${RED}Python3 not found. Please install Python3 to run the frontend server.${NC}"
        kill $BACKEND_PID
        exit 1
    fi
    
    echo -e "\n${GREEN}Smart Pomodoro Scheduler is running!${NC}"
    echo "Access the application at http://localhost:8080"
    echo "Press Ctrl+C to stop all servers"
    
    # Wait for Ctrl+C
    trap "echo -e '\n${BLUE}Shutting down servers...${NC}' && kill $BACKEND_PID $FRONTEND_PID" INT
    wait
else
    echo -e "${RED}Backend build failed!${NC}"
    exit 1
fi