#ifndef SORT_HPP
#define SORT_HPP

// Standard C++ includes

// ROOT includes

// Project includes
#include "HistogramManager.hpp"

// Forward declarations
class Event;
class Experiment;
class Run;

namespace Sort
{

    // Function to fill histograms with raw data from the event
    void fillRawDataHistograms(Event &event, HistogramManager::HistogramPtrMap &hist_ptr_map);

    // Function to sort a run
    // Valid Modes are "all", "raw", "calibrated", "energy", "addback".
    void sortRun(Experiment &experiment, Run &run, HistogramManager &hist_man, std::string mode = "all");

}

#endif // SORT_HPP