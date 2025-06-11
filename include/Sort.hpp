#ifndef SORT_HPP
#define SORT_HPP

class Event;
class HistogramManager;
namespace Sort
{
    // Function to fill histograms with raw data from the event
    void fillRawDataHistograms(Event *event, HistogramManager *hist_manager);
}

#endif // SORT_HPP