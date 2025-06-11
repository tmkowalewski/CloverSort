/*  Interface class for Tasks
    This allows for Task instances to be stored within the same container despite their different types.
*/

#ifndef ITASK_HPP
#define ITASK_HPP

#include <TString.h>

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