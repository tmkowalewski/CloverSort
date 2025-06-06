#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <vector>

// Forward declarations

class Task;

class TaskManager
{
public:
    // Default constructor
    TaskManager() = default;

    // Default destructor
    virtual ~TaskManager() = default;

    // Getters

    const std::vector<Task *> &getTasks() const { return tasks_; }

    virtual void initialize();
    virtual void execute();
    virtual void finalize();

protected:
    std::vector<Task *> tasks_; // List of tasks to manage
};

#endif // TASK_MANAGER_HPP