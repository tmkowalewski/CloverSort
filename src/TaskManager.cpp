#include "TaskManager.hpp"
#include "ITask.hpp"
#include <algorithm>

TaskManager::TaskManager() = default;

TaskManager::~TaskManager()
{
}

void TaskManager::initializeTasks()
{
    for (auto &task : tasks_)
    {
        task->callInitialize();
    }
}

void TaskManager::executeTasks()
{
    for (auto &task : tasks_)
    {
        task->callExecute();
    }
}

void TaskManager::finalizeTasks()
{
    for (auto &task : tasks_)
    {
        task->callFinalize();
    }
}

void TaskManager::addTask(ITask *task)
{
    if (task)
    {
        tasks_.push_back(task);
    }
}

void TaskManager::removeTask(ITask *task)
{
    auto it = std::remove(tasks_.begin(), tasks_.end(), task);
    if (it != tasks_.end())
    {
        tasks_.erase(it, tasks_.end());
    }
}