#ifndef DISPLAY_SCHEDULER_H
#define DISPLAY_SCHEDULER_H

#include <vector>
#include "FreeRTOS.h"
#include "map"
#include "Arduino.h"

enum class TaskPriority {
    Low = 1,
    Middle,
    High
};

typedef struct {
    String name;
    TaskFunction_t func;
    TaskPriority priority;
    int stackDepth;
    void *parameters;
    int core;
} TaskToSchedule;

class TaskScheduler {
protected:
    std::vector<TaskToSchedule> tasksToSchedule;
    std::vector<String> tasksToDelete;
    std::map<String, TaskHandle_t> inRunning;

    TaskHandle_t getTaskHandle(const String &taskName);

public:
    explicit TaskScheduler() {};

    void schedule();

    void addTask(const TaskToSchedule &t);

    void deleteTask(const String &name);
};

#endif
