#ifndef TASK_HPP
#define TASK_HPP

#include <functional>

class Task
{
public:
    // Default constructor
    Task() = default;

    // Default destructor
    virtual ~Task() = default;

    std::function<void()> taskFunction; // Function to execute the task
};

#endif // TASK_HPP