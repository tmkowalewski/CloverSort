#ifndef SORT_TASK_MANAGER_HPP
#define SORT_TASK_MANAGER_HPP

#include "TaskManager.hpp"
#include "ROOT/TTreeProcessorMT.hxx"

#define N_THREADS (std::thread::hardware_concurrency() - 2) // Number of threads to use for processing, defaults to system max - 2

// Forward declarations

class TTreeReader;
class Experiment;
class HistogramManager;
class Event;

class SortTaskManager : public TaskManager
{
public:
    // Constructor
    SortTaskManager(Experiment *pexperiment, HistogramManager *phistogram_manager);

    // Default destructor
    virtual ~SortTaskManager();

    // Override methods from the parent class
    virtual void initializeTasks() override;
    virtual void executeTasks() override;

protected:
    Experiment *pexperiment_;              // Pointer to the Experiment object, if needed
    HistogramManager *phistogram_manager_; // Example member for managing histograms, if needed
};

#endif // SORT_TASK_MANAGER_HPP