#include "TaskScheduler.h"
#include "stdexcept"

using namespace std;

void TaskScheduler::addTask(const TaskToSchedule &t) {
    bool exist = false;
    for (const auto &pair: inRunning) {
        if (pair.first == t.name) {
            exist = true;
            break;
        }
    }
    if (!exist) {
        tasksToSchedule.push_back(t);
    }
}

void TaskScheduler::schedule() {
    for (const auto &task: tasksToSchedule) {
        TaskHandle_t handle = getTaskHandle(task.name);
        if (handle == nullptr) {
            int code;
            if (task.core != -1) {
                code = xTaskCreatePinnedToCore(
                        task.func,
                        task.name.c_str(),
                        task.stackDepth,
                        task.parameters,
                        (int) task.priority,
                        &handle,
                        task.core
                );
            } else {
                code = xTaskCreate(
                        task.func,
                        task.name.c_str(),
                        task.stackDepth,
                        task.parameters,
                        (int) task.priority,
                        &handle
                );
            }
            if (code != pdPASS) {
                throw runtime_error("Create task error");
            }
        } else {
            vTaskResume(handle);
        }
        inRunning.insert(std::make_pair(task.name, handle));
    }
    tasksToSchedule.clear();

    for (const auto &task: tasksToDelete) {
        auto handle = getTaskHandle(task);
        if (handle != nullptr) {
            vTaskSuspend(handle);
            inRunning.erase(task);
        }
    }
    tasksToDelete.clear();
}

void TaskScheduler::deleteTask(const String &name) {
    for (const auto &pair: inRunning) {
        if (pair.first == name) {
            tasksToDelete.push_back(pair.first);
            break;
        }
    }
}

TaskHandle_t TaskScheduler::getTaskHandle(const String &taskName) {
    try {
        return inRunning.at(taskName);
    } catch (out_of_range &e) {
        return nullptr;
    }
}
