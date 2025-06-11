#include <TTreeReader.h>
#include "SortTaskManager.hpp"
#include "Event.hpp"
#include "Experiment.hpp"
#include "Run.hpp"
#include "ITask.hpp"

SortTaskManager::SortTaskManager(Experiment *pexperiment, HistogramManager *phistogram_manager)
    : pexperiment_(pexperiment), phistogram_manager_(phistogram_manager)
{
}

SortTaskManager::~SortTaskManager()
{
}

void SortTaskManager::initializeTasks()
{
}

void SortTaskManager::executeTasks()
{
}
