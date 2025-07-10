#ifndef SORT_HPP
#define SORT_HPP

// Forward declarations
class Event;
class Experiment;
class Run;
class HistogramManager;

namespace Sort
{
    // Function to sort a run
    void sortRun(Experiment &experiment, Run &run);

    // Function to fill histograms with raw data from the event
    void fillRawDataHistograms(Event &event, Experiment &experiment, HistogramManager &hist_manager);

    // Function which is run on every thread to actually loop through the events
    // and fill the histograms
    void sortTask(TTreeReader &event_reader);
}

#endif // SORT_HPP