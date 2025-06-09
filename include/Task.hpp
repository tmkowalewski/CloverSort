#ifndef TASK_HPP
#define TASK_HPP

#include <functional>
#include <utility>
#include <stdexcept>
#include <tuple>       // Required for std::tuple and std::apply
#include <type_traits> // Required for std::is_void_v, std::conditional_t
#include <optional>    // Required for std::optional

#include "ITask.hpp" // Assuming ITask might be used or inherited from

// FunctionSignatureTraits is a utility to extract argument and return types from function signatures.
namespace detail
{
    template <typename FuncSignature>
    struct FunctionSignatureTraits;

    // Specialization for function signatures like Ret(Args...)
    template <typename Ret, typename... Args>
    struct FunctionSignatureTraits<Ret(Args...)>
    {
        using ArgsTuple = std::tuple<Args...>;
        using ReturnType = Ret;
    };
}

// Helper struct for void return types in std::conditional_t
struct EmptyResult
{
};

template <typename InitFuncSignature, typename ExecFuncSignature, typename FinalFuncSignature>
class Task : public ITask
{
public:
    // Function signature types (passed as template parameters)
    using InitializeFuncStd = std::function<InitFuncSignature>;
    using ExecuteFuncStd = std::function<ExecFuncSignature>;
    using FinalizeFuncStd = std::function<FinalFuncSignature>;

    // Argument tuple types derived from signatures
    using InitializeArgsTuple = typename detail::FunctionSignatureTraits<InitFuncSignature>::ArgsTuple;
    using ExecuteArgsTuple = typename detail::FunctionSignatureTraits<ExecFuncSignature>::ArgsTuple;
    using FinalizeArgsTuple = typename detail::FunctionSignatureTraits<FinalFuncSignature>::ArgsTuple;

    // Return types derived from signatures
    using InitializeReturnType = typename detail::FunctionSignatureTraits<InitFuncSignature>::ReturnType;
    using ExecuteReturnType = typename detail::FunctionSignatureTraits<ExecFuncSignature>::ReturnType;
    using FinalizeReturnType = typename detail::FunctionSignatureTraits<FinalFuncSignature>::ReturnType;

    // Output storage types: std::optional<ReturnType> for non-void, EmptyResult for void
    using InitializeOutputStorage = std::conditional_t<std::is_void_v<InitializeReturnType>, EmptyResult, std::optional<InitializeReturnType>>;
    using ExecuteOutputStorage = std::conditional_t<std::is_void_v<ExecuteReturnType>, EmptyResult, std::optional<ExecuteReturnType>>;
    using FinalizeOutputStorage = std::conditional_t<std::is_void_v<FinalizeReturnType>, EmptyResult, std::optional<FinalizeReturnType>>;

    // Constructor accepts functions and their corresponding arguments
    Task(TString name, InitializeFuncStd initFunc, ExecuteFuncStd execFunc, FinalizeFuncStd finalFunc,
         InitializeArgsTuple initArgs = InitializeArgsTuple{},
         ExecuteArgsTuple execArgs = ExecuteArgsTuple{},
         FinalizeArgsTuple finalArgs = FinalizeArgsTuple{})
        : name_(name), initialize_func_(std::move(initFunc)), execute_func_(std::move(execFunc)), finalize_func_(std::move(finalFunc)),
          initialize_args_(std::move(initArgs)), execute_args_(std::move(execArgs)), finalize_args_(std::move(finalArgs)) {}

    // Optional: default constructor
    Task() = default;

    // Getters

    const TString &getName() const { return name_; }

    const InitializeFuncStd &getInitializeFunction() const { return initialize_func_; }
    const ExecuteFuncStd &getExecuteFunction() const { return execute_func_; }
    const FinalizeFuncStd &getFinalizeFunction() const { return finalize_func_; }

    const InitializeArgsTuple &getInitializeArguments() const { return initialize_args_; }
    const ExecuteArgsTuple &getExecuteArguments() const { return execute_args_; }
    const FinalizeArgsTuple &getFinalizeArguments() const { return finalize_args_; }

    template <typename T = InitializeReturnType, typename = std::enable_if_t<!std::is_void_v<T>>>
    const std::optional<T> &getInitializeOutput() const
    {
        static_assert(!std::is_void_v<InitializeReturnType>, "Cannot get output for void return type.");
        return initialize_output_;
    }

    template <typename T = ExecuteReturnType, typename = std::enable_if_t<!std::is_void_v<T>>>
    const std::optional<T> &getExecuteOutput() const
    {
        static_assert(!std::is_void_v<ExecuteReturnType>, "Cannot get output for void return type.");
        return execute_output_;
    }

    template <typename T = FinalizeReturnType, typename = std::enable_if_t<!std::is_void_v<T>>>
    const std::optional<T> &getFinalizeOutput() const
    {
        static_assert(!std::is_void_v<FinalizeReturnType>, "Cannot get output for void return type.");
        return finalize_output_;
    }

    // Setters

    void setName(const TString &name) { name_ = name; }

    void setInitializeFunction(InitializeFuncStd func) { initialize_func_ = std::move(func); }
    void setExecuteFunction(ExecuteFuncStd func) { execute_func_ = std::move(func); }
    void setFinalizeFunction(FinalizeFuncStd func) { finalize_func_ = std::move(func); }

    void setInitializeArguments(InitializeArgsTuple args) { initialize_args_ = std::move(args); }
    void setExecuteArguments(ExecuteArgsTuple args) { execute_args_ = std::move(args); }
    void setFinalizeArguments(FinalizeArgsTuple args) { finalize_args_ = std::move(args); }

    // Callers use stored arguments and store outputs
    void callInitialize()
    {
        if (!initialize_func_)
            throw std::runtime_error("Initialize function not set");

        if constexpr (!std::is_void_v<InitializeReturnType>)
        {
            initialize_output_ = std::apply(initialize_func_, initialize_args_);
        }
        else
        {
            std::apply(initialize_func_, initialize_args_);
            // initialize_output_ is EmptyResult, nothing to assign
        }
    }

    void callExecute()
    {
        if (!execute_func_)
            throw std::runtime_error("Execute function not set");

        if constexpr (!std::is_void_v<ExecuteReturnType>)
        {
            execute_output_ = std::apply(execute_func_, execute_args_);
        }
        else
        {
            std::apply(execute_func_, execute_args_);
            // execute_output_ is EmptyResult, nothing to assign
        }
    }

    void callFinalize()
    {
        if (!finalize_func_)
            throw std::runtime_error("Finalize function not set");

        if constexpr (!std::is_void_v<FinalizeReturnType>)
        {
            finalize_output_ = std::apply(finalize_func_, finalize_args_);
        }
        else
        {
            std::apply(finalize_func_, finalize_args_);
            // finalize_output_ is EmptyResult, nothing to assign
        }
    }

protected:
    TString name_; // Optional: name of the task, can be used for identification

    // Stored functions
    InitializeFuncStd initialize_func_;
    ExecuteFuncStd execute_func_;
    FinalizeFuncStd finalize_func_;

    // Stored arguments
    InitializeArgsTuple initialize_args_;
    ExecuteArgsTuple execute_args_;
    FinalizeArgsTuple finalize_args_;

    // Stored outputs
    InitializeOutputStorage initialize_output_;
    ExecuteOutputStorage execute_output_;
    FinalizeOutputStorage finalize_output_;
};

#endif // TASK_HPP