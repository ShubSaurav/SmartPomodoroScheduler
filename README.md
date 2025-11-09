# Smart Pomodoro Scheduler

A smart task scheduling and time management application that combines the Pomodoro Technique with intelligent task prioritization. Built with C++ backend and a modern web frontend.

## Features

- **Intelligent Task Scheduling**: Automatically optimizes your task schedule based on priority, duration, and deadlines
- **Pomodoro Timer**: Built-in Pomodoro timer with customizable work/break intervals
- **Task Management**: Add, view, delete, and mark tasks as complete
- **Priority System**: Four-level priority system (Critical, High, Medium, Low)
- **Urgency Scoring**: Dynamic urgency calculation based on deadlines and priority
- **Dual Interface**: Both CLI and GUI versions available
- **Persistent Storage**: Save and load tasks from files
- **Task Statistics**: View productivity statistics and track progress

## Technology Stack

### Backend
- **Language**: C++17
- **Components**: 
  - HTTP Server (custom implementation)
  - Task Scheduler with optimization algorithms
  - Pomodoro Timer
  - File-based data storage

### Frontend
- **HTML5/CSS3/JavaScript**
- **Simple HTTP Server** (Python 3)

## Prerequisites

Before running the application, ensure you have the following installed:

- **g++** compiler with C++17 support
  ```bash
  # On macOS (install via Xcode Command Line Tools)
  xcode-select --install
  
  # On Linux (Ubuntu/Debian)
  sudo apt-get install g++
  ```

- **Python 3** (for frontend server)
  ```bash
  # Check if installed
  python3 --version
  
  # On macOS
  brew install python3
  
  # On Linux (Ubuntu/Debian)
  sudo apt-get install python3
  ```

- **Make** (optional, but recommended)
  ```bash
  # Usually comes with build-essential
  sudo apt-get install build-essential
  ```

## Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/ShubSaurav/SmartPomodoroScheduler.git
   cd SmartPomodoroScheduler
   ```

2. **Make scripts executable**
   ```bash
   chmod +x run.sh run_cli.sh run_gui.sh
   ```

3. **Remove quarantine attributes (macOS only)**
   ```bash
   xattr -d com.apple.quarantine run.sh run_cli.sh run_gui.sh 2>/dev/null
   ```

## Usage

### Running the GUI Version (Recommended)

The GUI version provides a complete web interface with visual task management.

```bash
./run_gui.sh
```

Or with bash:
```bash
bash run_gui.sh
```

The application will:
1. Build the C++ backend
2. Start the backend server on port 8080
3. Start the frontend HTTP server
4. Automatically open your browser to `http://localhost:8080`

**To stop the servers**: Press `Ctrl+C` in the terminal

### Running the CLI Version

The CLI version provides a text-based interface for terminal users.

```bash
./run_cli.sh
```

Or with bash:
```bash
bash run_cli.sh
```

### Running Both with Custom Options

```bash
# For GUI version
./run.sh

# For CLI version
./run.sh cli
```

## Project Structure

```
SmartPomodoroScheduler/
├── backend/                 # C++ backend source code
│   ├── server.cpp          # HTTP server main file
│   ├── cli.cpp             # CLI interface
│   ├── HTTPServer.cpp/h    # HTTP server implementation
│   ├── Task.cpp/h          # Task model
│   ├── Scheduler.cpp/h     # Scheduling algorithms
│   ├── PomodoroTimer.cpp/h # Timer implementation
│   └── Utils.cpp/h         # Utility functions
├── frontend/               # Web frontend
│   ├── index.html         # Main HTML page
│   ├── style.css          # Styling
│   └── script.js          # Frontend logic
├── data/                   # Data storage
│   ├── task.txt           # Task data
│   └── schedule.txt       # Optimized schedule
├── config/                 # Configuration files
├── run.sh                 # Main run script
├── run_gui.sh             # GUI version launcher
├── run_cli.sh             # CLI version launcher
└── README.md              # This file
```

## CLI Menu Options

When running the CLI version, you'll have access to:

1. **Add Task** - Create a new task with name, duration, priority, and deadline
2. **List Tasks** - View all tasks with their details
3. **Optimize Schedule** - Run the scheduling algorithm
4. **Start Pomodoro Timer** - Begin a Pomodoro session
   - 25/5/15 (work/short break/long break)
   - 45/15/30 
   - Custom intervals
5. **Save Tasks** - Save current tasks to file
6. **Load Tasks** - Load tasks from file
7. **Delete Task** - Remove a task
8. **Mark Task as Complete** - Mark task as done
9. **View Task Statistics** - See productivity stats
0. **Exit** - Quit the application

## API Endpoints

The backend server exposes the following REST API endpoints:

- `GET /` - Health check
- `GET /tasks` - Retrieve all tasks
- `POST /optimize` - Optimize task schedule

## Scheduling Algorithm

The scheduler uses an intelligent algorithm that considers:
- **Task Priority**: Critical > High > Medium > Low
- **Deadline Proximity**: Tasks closer to deadline get higher urgency
- **Duration**: Efficient time allocation
- **Urgency Score**: Dynamically calculated based on all factors

## Troubleshooting

### Port Already in Use

If you get an "Address already in use" error:

```bash
# Find and kill processes on port 8080
lsof -ti:8080 | xargs kill -9

# Or use a different port (edit the scripts)
```

### Permission Denied (macOS)

```bash
# Remove quarantine attribute
xattr -d com.apple.quarantine run_gui.sh run_cli.sh run.sh

# Make executable
chmod +x run_gui.sh run_cli.sh run.sh

# Run with bash explicitly
bash run_gui.sh
```

### Build Errors

If you encounter build errors:

1. Ensure g++ supports C++17:
   ```bash
   g++ --version
   ```

2. Check for missing dependencies:
   ```bash
   # macOS
   xcode-select --install
   
   # Linux
   sudo apt-get install build-essential
   ```

### Frontend Not Loading

1. Check if Python 3 is installed:
   ```bash
   python3 --version
   ```

2. Ensure port 8080 is not blocked by firewall

3. Try accessing directly: `http://localhost:8080`

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## Future Enhancements

- [ ] Task categories and tags
- [ ] Calendar integration
- [ ] Data visualization and analytics
- [ ] Mobile app version
- [ ] Cloud sync functionality
- [ ] Notification system
- [ ] Task dependencies
- [ ] Team collaboration features

## License

This project is open source and available under the [MIT License](LICENSE).

## Author

**Shubham Saurav** - [@ShubSaurav](https://github.com/ShubSaurav)

## Acknowledgments

- Inspired by the Pomodoro Technique® by Francesco Cirillo
- Built as part of productivity tools collection
- Thanks to all contributors and users

## Support

If you encounter any issues or have questions:
- Open an issue on GitHub
- Contact: shubhamsaurav2264@gmail.com

---

**Happy Productive Coding! 🍅⏱️**
