#ifndef TASK_HPP
#define TASK_HPP

#include <functional>

class Event;

class Task
{
public:
    // Default constructor
    Task() = default;

    // Default destructor
    virtual ~Task() = default;

    std::function<void()> initializeFunction;          // Function to execute the task
    std::function<void(Event &event)> executeFunction; // Function to execute the task
    std::function<void()> finalizeFunction;            // Function to finalize the task
};

#endif // TASK_HPP