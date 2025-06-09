#ifndef ITASK_HPP
#define ITASK_HPP

class ITask
{
public:
    virtual ~ITask() = default;

    virtual void callInitialize() = 0;
    virtual void callExecute() = 0;
    virtual void callFinalize() = 0;
    virtual const TString &getName() const = 0;
};
#endif // ITASK_HPP