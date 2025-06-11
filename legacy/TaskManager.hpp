#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <vector>
#include "Task.hpp"

template <typename ProcessingArgs, typename... Args>
class TaskManager
{
public:
    using InitializeFuncStd = ProcessingArgs(Args...);
    using ExecuteFuncStd = void(ProcessingArgs);
    using FinalizeFuncStd = void(ProcessingArgs);
    using TaskType = Task<InitializeFuncStd, ExecuteFuncStd, FinalizeFuncStd>;
    using TaskVector = std::vector<TaskType *>; // Vector of pointers to Task objects
    // Default constructor
    TaskManager() = default;

    // Default destructor
    virtual ~TaskManager()
    {
        for (TaskType *task : tasks_)
        {
            delete task;
        }
        tasks_.clear();
    }

    // Getters

    const TaskVector &getTasks() const { return tasks_; }

    virtual void initializeTasks()
    {
        for (auto &task : tasks_)
        {
            task->callInitialize(); // This should populate the task's initialize_output_

            const auto &init_output_optional = task->getInitializeOutput();

            if (init_output_optional)
            {
                task->setExecuteArguments(std::make_tuple(*init_output_optional));
            }
            else
            {
                if (task->getName())
                {
                    throw std::runtime_error("Task " + std::string(task->getName().Data()) + " failed to produce initialize output for execute step.");
                }
                else
                {
                    throw std::runtime_error("A Task has failed to produce initialize output for execute step.");
                }
            }
        }
    }

    virtual void executeTasks()
    {
        for (auto &task : tasks_)
        {
            task->callExecute();
        }
    }

    virtual void finalizeTasks()
    {
        for (auto &task : tasks_)
        {
            task->callFinalize();
        }
    }

    virtual void performTasks()
    {
        initializeTasks();
        executeTasks();
        finalizeTasks();
    }

    virtual void addTask(TaskType *task)
    {
        if (task)
        {
            tasks_.push_back(task);
        }
    }

    virtual void addTask(const TString &name, InitializeFuncStd initFunc, ExecuteFuncStd execFunc, FinalizeFuncStd finalFunc)
    {
        tasks_.emplace_back(new TaskType(name, std::move(initFunc), std::move(execFunc), std::move(finalFunc)));
    }

    virtual void removeTask(const TString &name)
    {
        auto it = std::remove_if(tasks_.begin(), tasks_.end(),
                                 [&name](TaskType *task)
                                 { return task->getName() == name; });
        if (it != tasks_.end())
        {
            delete *it; // Clean up memory
            tasks_.erase(it, tasks_.end());
        }
    }

protected:
    TaskVector tasks_; // List of tasks to manage
};

#endif // TASK_MANAGER_HPP