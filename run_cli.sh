#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}Smart Pomodoro Scheduler - CLI Version${NC}"

# Change to the backend directory
cd "$(dirname "$0")/backend"

# Build the CLI version
echo -e "\n${BLUE}Building CLI version...${NC}"
g++ -std=c++17 \
    cli.cpp \
    Task.cpp \
    Scheduler.cpp \
    PomodoroTimer.cpp \
    Utils.cpp \
    -I/usr/local/include \
    -L/usr/local/lib \
    -lpthread \
    -o SmartPomodoroSchedulerCLI

# Check if build was successful
if [ $? -eq 0 ]; then
    echo -e "${GREEN}CLI version built successfully!${NC}"
    # Run the CLI version
    ./SmartPomodoroSchedulerCLI
else
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi