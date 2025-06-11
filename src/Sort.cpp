#include "Sort.hpp"
#include "Experiment.hpp"
#include "Event.hpp"
#include "HistogramManager.hpp"

void Sort::fillRawDataHistograms(Event &event, Experiment &experiment, HistogramManager::HistogramPtrMap &hist_ptr_map)
{
    for (const auto &key : hist_ptr_map)
    {
        const std::vector<TString> &histogram_path = key.first;
        const std::vector<std::shared_ptr<TH1D>> &histograms = key.second;

        for (const auto &histogram : histograms)
        {
            // Assuming the histogram name is the filter to get data from the event
            TString filter = histogram->GetName();
            Double_t data_value = event.GetData(nullptr, filter);

            if (data_value != 0) // Assuming we only fill non-zero values
            {
                histogram->Fill(data_value);
            }
        }
    }
}
