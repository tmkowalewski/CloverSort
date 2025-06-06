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
        task.callInitialize();
    }
}

void TaskManager::executeTasks()
{
    for (auto &task : tasks_)
    {
        task.callExecute();
    }
}

void TaskManager::finalizeTasks()
{
    for (auto &task : tasks_)
    {
        task.callFinalize();
    }
}