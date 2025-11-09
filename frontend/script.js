let timerInterval;
let timeLeft = 0;
let isRunning = false;
let currentMode = 'pomodoro';
let activeTask = null;

const timerEl = document.getElementById("timer");
const taskCards = document.getElementById("taskCards");
const taskName = document.getElementById("taskName");
const taskPriority = document.getElementById("taskPriority");

// Task Management
let currentFilter = 'all';

// Timer Mode Management
document.querySelectorAll('.mode-btn').forEach(btn => {
  btn.addEventListener('click', () => {
    if (isRunning) {
      if (!confirm('Timer is running. Switch mode?')) return;
      stopTimer();
    }
    
    currentMode = btn.dataset.mode;
    document.querySelectorAll('.mode-btn').forEach(b => b.classList.remove('active'));
    btn.classList.add('active');
    
    // Reset timer
    timeLeft = 0;
    updateTimer();
    
    // Update UI based on mode
    if (currentMode === 'pomodoro') {
      document.getElementById('pomodoroPresets').style.display = 'flex';
      if (!activeTask) {
        document.getElementById('activeTaskInfo').innerHTML = '<p>No active task</p>';
      }
    } else {
      document.getElementById('pomodoroPresets').style.display = 'none';
    }
  });
});

// Timer Controls
document.getElementById('startBtn').addEventListener('click', () => {
  if (timeLeft === 0) {
    if (currentMode === 'pomodoro') {
      timeLeft = parseInt(document.getElementById('customTime').value) * 60;
    } else if (!activeTask) {
      showNotification('Please select a task first!', 'error');
      return;
    }
  }
  startTimer();
});

document.getElementById('pauseBtn').addEventListener('click', () => {
  if (isRunning) {
    clearInterval(timerInterval);
    isRunning = false;
    document.getElementById('startBtn').innerHTML = '<i class="fas fa-play"></i>';
  }
});

document.getElementById('resetBtn').addEventListener('click', () => {
  if (confirm('Reset timer?')) {
    stopTimer();
    if (currentMode === 'pomodoro') {
      timeLeft = parseInt(document.getElementById('customTime').value) * 60;
    } else if (activeTask) {
      timeLeft = activeTask.estimatedTime * 60;
    } else {
      timeLeft = 0;
    }
    updateTimer();
  }
});

// Timer Presets
document.querySelectorAll('.preset-btn').forEach(btn => {
  btn.addEventListener('click', () => {
    const minutes = parseInt(btn.dataset.time);
    document.getElementById('customTime').value = minutes;
    document.getElementById('sliderValue').textContent = `${minutes} min`;
    if (!isRunning) {
      timeLeft = minutes * 60;
      updateTimer();
    }
  });
});

// Load tasks from localStorage on startup
window.addEventListener("load", () => {
  refreshAllTasks();
  updateTimer();
  updateActiveTaskInfo(null);
  setupTaskFilters();
  
  // Initialize timer slider
  const customTimeSlider = document.getElementById('customTime');
  const sliderValue = document.getElementById('sliderValue');
  
  customTimeSlider.addEventListener('input', (e) => {
    const minutes = e.target.value;
    sliderValue.textContent = `${minutes} min`;
    if (!isRunning) {
      timeLeft = minutes * 60;
      updateTimer();
    }
  });
});

// Add new task
document.getElementById("addTask").addEventListener("click", () => {
  const name = taskName.value.trim();
  const priority = taskPriority.value;
  const estimatedTime = document.getElementById("taskEstimatedTime").value;
  
  if (!name) {
    showNotification("Please enter a task name!", "error");
    return;
  }

  const newTask = {
    id: Date.now(),
    name,
    priority,
    estimatedTime: estimatedTime || "0",
    createdAt: new Date().toISOString(),
    completed: false
  };

  addTaskCard(newTask);
  saveTask(newTask);
  resetTaskForm();
  showNotification("Task added successfully!", "success");
});

// Save tasks persistently
function saveTask(task) {
  const tasks = JSON.parse(localStorage.getItem("tasks")) || [];
  tasks.push(task);
  localStorage.setItem("tasks", JSON.stringify(tasks));
}

// Reset task form
function resetTaskForm() {
  taskName.value = "";
  taskPriority.value = "Medium";
  document.getElementById("taskEstimatedTime").value = "";
}

// Show notification
function showNotification(message, type) {
  const notification = document.createElement("div");
  notification.className = `notification ${type}`;
  notification.innerHTML = `
    <i class="fas fa-${type === 'success' ? 'check-circle' : 'exclamation-circle'}"></i>
    ${message}
  `;
  document.body.appendChild(notification);
  setTimeout(() => notification.remove(), 3000);
}

// Render a task card
function addTaskCard(task) {
  if (currentFilter !== 'all' && task.priority !== currentFilter) return;
  
  const card = document.createElement("div");
  card.className = `card priority-${task.priority.toLowerCase()}${task.active ? ' active-task' : ''}`;
  card.innerHTML = `
    <div class="card-header">
      <span class="priority-badge">${task.priority}</span>
      <div class="card-actions">
        ${!task.completed ? `
          <button class="start-btn" title="Start Timer">
            <i class="fas fa-play"></i>
          </button>
        ` : ''}
        <button class="edit-btn" title="Edit Task">
          <i class="fas fa-edit"></i>
        </button>
        <button class="delete-btn" title="Delete Task">
          <i class="fas fa-trash"></i>
        </button>
      </div>
    </div>
    <h3 class="task-name">${task.name}</h3>
    <div class="task-details">
      <span>
        <i class="fas fa-clock"></i>
        ${task.estimatedTime} min
      </span>
      <span class="task-status ${task.completed ? 'completed' : ''}">
        <i class="fas fa-${task.completed ? 'check-circle' : 'circle'}"></i>
        ${task.completed ? 'Completed' : 'Pending'}
      </span>
    </div>
    ${task.active ? `
    <div class="timer-controls">
      <button class="timer-btn pause-btn">
        <i class="fas fa-pause"></i> Pause
      </button>
      <button class="timer-btn reset-btn">
        <i class="fas fa-undo"></i> Reset
      </button>
      <button class="timer-btn complete-btn">
        <i class="fas fa-check"></i> Done
      </button>
    </div>
    ` : ''}
  `;

  setupCardActions(card, task);
  taskCards.appendChild(card);
}

// Setup card actions
function setupCardActions(card, task) {
  const deleteBtn = card.querySelector(".delete-btn");
  const editBtn = card.querySelector(".edit-btn");
  const startBtn = card.querySelector(".start-btn");
  
  if (deleteBtn) {
    deleteBtn.addEventListener("click", () => {
      if (confirm("Are you sure you want to delete this task?")) {
        if (task.active) {
          stopTimer();
        }
        deleteTask(task.id);
        card.remove();
        showNotification("Task deleted successfully!", "success");
      }
    });
  }

  if (editBtn) {
    editBtn.addEventListener("click", () => {
      if (!task.active) {
        editTask(task, card);
      } else {
        showNotification("Please complete or stop the active task first!", "error");
      }
    });
  }

  if (startBtn) {
    startBtn.addEventListener("click", () => {
      if (isRunning) {
        if (confirm("Another task timer is running. Do you want to switch to this task?")) {
          stopTimer();
        } else {
          return;
        }
      }
      startTaskTimer(task, card);
    });
  }

  // Timer control buttons
  if (task.active) {
    const pauseBtn = card.querySelector(".pause-btn");
    const resetBtn = card.querySelector(".reset-btn");
    const completeBtn = card.querySelector(".complete-btn");

    pauseBtn.addEventListener("click", () => {
      if (isRunning) {
        clearInterval(timerInterval);
        isRunning = false;
        pauseBtn.innerHTML = '<i class="fas fa-play"></i> Resume';
      } else {
        startTimer();
        pauseBtn.innerHTML = '<i class="fas fa-pause"></i> Pause';
      }
    });

    resetBtn.addEventListener("click", () => {
      if (confirm("Are you sure you want to reset the timer?")) {
        timeLeft = task.estimatedTime * 60;
        updateTimer();
      }
    });

    completeBtn.addEventListener("click", () => {
      if (confirm("Mark this task as completed?")) {
        stopTimer();
        moveToCompleted(task);
        showNotification("Task completed successfully!", "success");
      }
    });
  }
}

// Start task timer
function startTaskTimer(task, card) {
  // Reset any existing active tasks
  const tasks = JSON.parse(localStorage.getItem("tasks")) || [];
  tasks.forEach(t => t.active = false);
  
  // Set this task as active
  task.active = true;
  activeTask = task;
  updateTask(task);
  
  // Switch to task mode
  currentMode = 'task';
  document.querySelectorAll('.mode-btn').forEach(btn => {
    btn.classList.toggle('active', btn.dataset.mode === 'task');
  });
  document.getElementById('pomodoroPresets').style.display = 'none';
  
  // Set timer
  timeLeft = task.estimatedTime * 60;
  updateTimer();
  startTimer();
  
  // Update active task info
  updateActiveTaskInfo(task);
  
  // Refresh the task list to update UI
  refreshTaskList();
}

function updateActiveTaskInfo(task) {
  const activeTaskInfo = document.getElementById('activeTaskInfo');
  if (task && task.active) {
    activeTaskInfo.innerHTML = `
      <div class="active-task-details">
        <h3>${task.name}</h3>
        <p><i class="fas fa-flag"></i> ${task.priority}</p>
        <p><i class="fas fa-clock"></i> ${task.estimatedTime} minutes</p>
      </div>
    `;
  } else {
    activeTaskInfo.innerHTML = '<p>No active task</p>';
  }
}

// Start timer
function startTimer() {
  isRunning = true;
  timerInterval = setInterval(() => {
    if (timeLeft > 0) {
      timeLeft--;
      updateTimer();
    } else {
      stopTimer();
      showNotification("Time's up! Task timer completed.", "success");
    }
  }, 1000);
}

// Stop timer
function stopTimer() {
  clearInterval(timerInterval);
  isRunning = false;
  timeLeft = 0;
  updateTimer();
  
  // Clear active state from all tasks
  const tasks = JSON.parse(localStorage.getItem("tasks")) || [];
  let updated = false;
  tasks.forEach(task => {
    if (task.active) {
      task.active = false;
      updated = true;
    }
  });
  if (updated) {
    localStorage.setItem("tasks", JSON.stringify(tasks));
    refreshTaskList();
  }
}

// Edit task
function editTask(task, card) {
  card.innerHTML = `
    <div class="edit-form">
      <input type="text" class="edit-name" value="${task.name}">
      <select class="edit-priority">
        ${['Critical', 'High', 'Medium', 'Low'].map(p => 
          `<option value="${p}" ${p === task.priority ? 'selected' : ''}>${p}</option>`
        ).join('')}
      </select>
      <input type="number" class="edit-time" value="${task.estimatedTime}">
      <div class="edit-actions">
        <button class="save-edit"><i class="fas fa-check"></i></button>
        <button class="cancel-edit"><i class="fas fa-times"></i></button>
      </div>
    </div>
  `;

  const saveBtn = card.querySelector(".save-edit");
  const cancelBtn = card.querySelector(".cancel-edit");

  saveBtn.addEventListener("click", () => {
    const updatedTask = {
      ...task,
      name: card.querySelector(".edit-name").value,
      priority: card.querySelector(".edit-priority").value,
      estimatedTime: card.querySelector(".edit-time").value
    };
    updateTask(updatedTask);
    card.remove();
    addTaskCard(updatedTask);
    showNotification("Task updated successfully!", "success");
  });

  cancelBtn.addEventListener("click", () => {
    card.remove();
    addTaskCard(task);
  });
}

// Update task
function updateTask(updatedTask) {
  const tasks = JSON.parse(localStorage.getItem("tasks")) || [];
  const index = tasks.findIndex(t => t.id === updatedTask.id);
  if (index !== -1) {
    tasks[index] = updatedTask;
    localStorage.setItem("tasks", JSON.stringify(tasks));
  }
}

// Delete task
function deleteTask(id) {
  const tasks = JSON.parse(localStorage.getItem("tasks")) || [];
  const filtered = tasks.filter(t => t.id !== id);
  localStorage.setItem("tasks", JSON.stringify(filtered));
}

// Move task to completed
function moveToCompleted(task) {
  const tasks = JSON.parse(localStorage.getItem("tasks")) || [];
  const completedTasks = JSON.parse(localStorage.getItem("completedTasks")) || [];
  
  // Remove from active tasks
  const updatedTasks = tasks.filter(t => t.id !== task.id);
  localStorage.setItem("tasks", JSON.stringify(updatedTasks));
  
  // Add to completed tasks
  task.completedAt = new Date().toISOString();
  completedTasks.push(task);
  localStorage.setItem("completedTasks", JSON.stringify(completedTasks));
  
  refreshAllTasks();
}

// Render completed task card
function addCompletedTaskCard(task) {
  const card = document.createElement("div");
  card.className = `card completed priority-${task.priority.toLowerCase()}`;
  card.innerHTML = `
    <div class="card-header">
      <span class="priority-badge">${task.priority}</span>
      <div class="card-actions">
        <button class="redo-btn" title="Redo Task">
          <i class="fas fa-redo"></i>
        </button>
        <button class="delete-btn" title="Delete Task">
          <i class="fas fa-trash"></i>
        </button>
      </div>
    </div>
    <h3 class="task-name">${task.name}</h3>
    <div class="task-details">
      <span>
        <i class="fas fa-clock"></i>
        ${task.estimatedTime} min
      </span>
      <span class="completion-time">
        <i class="fas fa-check-circle"></i>
        ${formatCompletionTime(task.completedAt)}
      </span>
    </div>
  `;

  setupCompletedCardActions(card, task);
  document.getElementById("completedTaskCards").appendChild(card);
}

// Setup completed card actions
function setupCompletedCardActions(card, task) {
  card.querySelector(".delete-btn").addEventListener("click", () => {
    if (confirm("Are you sure you want to delete this completed task?")) {
      deleteCompletedTask(task.id);
      card.remove();
      showNotification("Completed task deleted", "success");
    }
  });

  card.querySelector(".redo-btn").addEventListener("click", () => {
    redoTask(task);
    card.remove();
    showNotification("Task moved back to active tasks", "success");
  });
}

// Delete completed task
function deleteCompletedTask(taskId) {
  const completedTasks = JSON.parse(localStorage.getItem("completedTasks")) || [];
  const filtered = completedTasks.filter(t => t.id !== taskId);
  localStorage.setItem("completedTasks", JSON.stringify(filtered));
}

// Redo task
function redoTask(task) {
  // Remove from completed tasks
  const completedTasks = JSON.parse(localStorage.getItem("completedTasks")) || [];
  const updatedCompleted = completedTasks.filter(t => t.id !== task.id);
  localStorage.setItem("completedTasks", JSON.stringify(updatedCompleted));
  
  // Add back to active tasks
  const newTask = {
    ...task,
    id: Date.now(), // New ID to avoid conflicts
    completed: false,
    active: false,
    completedAt: null
  };
  
  const tasks = JSON.parse(localStorage.getItem("tasks")) || [];
  tasks.push(newTask);
  localStorage.setItem("tasks", JSON.stringify(tasks));
  
  refreshAllTasks();
}

// Format completion time
function formatCompletionTime(timestamp) {
  const date = new Date(timestamp);
  return date.toLocaleDateString() + ' ' + date.toLocaleTimeString();
}

// Refresh all task lists
function refreshAllTasks() {
  // Refresh active tasks
  refreshTaskList();
  
  // Refresh completed tasks
  const completedTaskCards = document.getElementById("completedTaskCards");
  completedTaskCards.innerHTML = "";
  const completedTasks = JSON.parse(localStorage.getItem("completedTasks")) || [];
  completedTasks.sort((a, b) => new Date(b.completedAt) - new Date(a.completedAt));
  completedTasks.forEach(addCompletedTaskCard);
}

// Setup task filters
function setupTaskFilters() {
  const filterBtns = document.querySelectorAll(".filter-btn");
  
  filterBtns.forEach(btn => {
    btn.addEventListener("click", () => {
      currentFilter = btn.dataset.filter;
      filterBtns.forEach(b => b.classList.remove("active"));
      btn.classList.add("active");
      
      refreshTaskList();
    });
  });
}

// Refresh task list
function refreshTaskList() {
  taskCards.innerHTML = "";
  const tasks = JSON.parse(localStorage.getItem("tasks")) || [];
  tasks.forEach(addTaskCard);
}

// 🕒 Timer logic
document.querySelectorAll(".preset-buttons button").forEach(btn => {
  btn.addEventListener("click", () => {
    timeLeft = btn.getAttribute("data-min") * 60;
    updateTimer();
  });
});

document.getElementById("startBtn").addEventListener("click", () => {
  if (isRunning || timeLeft === 0) return;
  isRunning = true;
  timerInterval = setInterval(() => {
    if (timeLeft > 0) {
      timeLeft--;
      updateTimer();
    } else {
      clearInterval(timerInterval);
      alert("⏰ Time's up! Take a break!");
      isRunning = false;
    }
  }, 1000);
});

document.getElementById("pauseBtn").addEventListener("click", () => {
  clearInterval(timerInterval);
  isRunning = false;
});

document.getElementById("resetBtn").addEventListener("click", () => {
  clearInterval(timerInterval);
  timeLeft = 0;
  updateTimer();
  isRunning = false;
});

const customTimeSlider = document.getElementById("customTime");
const sliderValue = document.getElementById("sliderValue");

function formatTimeDisplay(totalMinutes) {
  const hours = Math.floor(totalMinutes / 60);
  const minutes = totalMinutes % 60;
  
  if (hours === 0) {
    return `${minutes} min`;
  } else if (minutes === 0) {
    return `${hours} ${hours === 1 ? 'hour' : 'hours'}`;
  } else {
    return `${hours} ${hours === 1 ? 'hour' : 'hours'} ${minutes} min`;
  }
}

customTimeSlider.addEventListener("input", (e) => {
  const minutes = parseInt(e.target.value);
  sliderValue.textContent = formatTimeDisplay(minutes);
  timeLeft = minutes * 60;
  updateTimer();
});

function updateTimer() {
  const min = String(Math.floor(timeLeft / 60)).padStart(2, "0");
  const sec = String(timeLeft % 60).padStart(2, "0");
  timerEl.textContent = `${min}:${sec}`;
}
