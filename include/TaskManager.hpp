#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <vector>

// Forward declarations
class ITask;

class TaskManager
{
public:
    // Default constructor
    TaskManager();

    // Default destructor
    virtual ~TaskManager();

    // Getters

    const std::vector<ITask> &getTasks() const { return tasks_; }

    virtual void initializeTasks();
    virtual void executeTasks();
    virtual void finalizeTasks();

protected:
    std::vector<ITask> tasks_; // List of tasks to manage
};

#endif // TASK_MANAGER_HPP