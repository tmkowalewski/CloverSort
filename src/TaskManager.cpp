#include <algorithm>
#include <TString.h>
#include "TaskManager.hpp"
#include "ITask.hpp"

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

void TaskManager::removeTask(const TString &name)
{
    auto it = std::remove_if(tasks_.begin(), tasks_.end(),
                             [&name](ITask *task)
                             { return task->getName() == name; });
    if (it != tasks_.end())
    {
        tasks_.erase(it, tasks_.end());
    }
}