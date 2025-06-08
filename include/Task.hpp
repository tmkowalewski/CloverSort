#ifndef TASK_HPP
#define TASK_HPP

#include <functional>
#include <utility>
#include <stdexcept>

#include "ITask.hpp"

template <typename InitFunc, typename ExecFunc, typename FinalFunc>
class Task
{
public:
    // Constructor accepts any callable compatible with the specified signatures
    Task(std::function<InitFunc> init,
         std::function<ExecFunc> exec,
         std::function<FinalFunc> final)
        : initialize_(std::move(init)),
          execute_(std::move(exec)),
          finalize_(std::move(final)) {}

    // Optional: default constructor
    Task() = default;

    // Getters
    const std::function<InitFunc> &getInitialize() const { return initialize_; }
    const std::function<ExecFunc> &getExecute() const { return execute_; }
    const std::function<FinalFunc> &getFinalize() const { return finalize_; }

    // Setters
    void setInitialize(std::function<InitFunc> func) { initialize_ = std::move(func); }
    void setExecute(std::function<ExecFunc> func) { execute_ = std::move(func); }
    void setFinalize(std::function<FinalFunc> func) { finalize_ = std::move(func); }

    // Callers (you provide arguments as needed)
    template <typename... Args>
    auto callInitialize(Args &&...args)
    {
        if (!initialize_)
            throw std::runtime_error("Initialize function not set");
        return initialize_(std::forward<Args>(args)...);
    }

    template <typename... Args>
    auto callExecute(Args &&...args)
    {
        if (!execute_)
            throw std::runtime_error("Execute function not set");
        return execute_(std::forward<Args>(args)...);
    }

    template <typename... Args>
    auto callFinalize(Args &&...args)
    {
        if (!finalize_)
            throw std::runtime_error("Finalize function not set");
        return finalize_(std::forward<Args>(args)...);
    }

protected:
    std::function<InitFunc> initialize_;
    std::function<ExecFunc> execute_;
    std::function<FinalFunc> finalize_;
};

#endif // TASK_HPP