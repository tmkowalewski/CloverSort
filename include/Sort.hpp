#ifndef SORT_HPP
#define SORT_HPP

class Event;
class Experiment;
class HistogramManager;

namespace Sort
{
    //
    void sortRun();

    // Function to fill histograms with raw data from the event
    void fillRawDataHistograms(Event &event, Experiment &experiment, HistogramManager &hist_manager);
}

#endif // SORT_HPP