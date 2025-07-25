// Standard C++ includes
#include <atomic>
#include <thread>

// ROOT includes
#include <ROOT/TTreeProcessorMT.hxx>
#include <TStopwatch.h>

// Project includes
#include "Sort.hpp"
#include "Experiment.hpp"
#include "DAQModule.hpp"
#include "Detector.hpp"
#include "Event.hpp"
#include "Run.hpp"
#include "Utilities.hpp"

namespace Sort
{

    void fillRawDataHistograms(Event &event, HistogramManager::HistogramPtrMap &hist_ptr_map)
    {
        // Module Loop
        for (const auto &pmodule : event.GetExperiment()->GetDAQModules())
        {
            const auto module_name = pmodule->GetName();

            // Module Timestamp Histogram
            const auto module_timestamp = event.GetData(module_name, "module_timestamp") * DAQ_BINS2NS; // Convert module timestamp to ns
            hist_ptr_map[module_name]["module_timestamp"][0]->Fill(module_timestamp);

            // Trigger Time Histograms
            const auto trigger_time0 = event.GetData(module_name, "trigger_time", 0) * DAQ_BINS2NS; // Convert trigger time to ns
            const auto trigger_time1 = event.GetData(module_name, "trigger_time", 1) * DAQ_BINS2NS; // Convert trigger time to ns
            hist_ptr_map[module_name]["trigger_time"][0]->Fill(trigger_time0);
            hist_ptr_map[module_name]["trigger_time"][1]->Fill(trigger_time1);

            for (const auto &filter : pmodule->GetFilters())
            {
                if (filter == "module_timestamp" || filter == "trigger_time")
                    continue; // Skip module_timestamp and trigger_time, as these are module filters handled above

                // Detector Loop
                for (const auto &detector : pmodule->GetDetectors())
                {
                    const auto detector_name = detector->GetName();
                    const auto detector_channels = detector->GetChannels();
                    const UInt_t det_chan_num = detector_channels.size();

                    // Channel Loop
                    for (const auto &channel : detector_channels)
                    {
                        const auto data_value = event.GetData(module_name, filter, channel);
                        hist_ptr_map[detector_name][filter][channel % det_chan_num + 1]->Fill(data_value); // Channel numbers are 0-indexed, so we add 1 to match histogram indexing
                    }
                }

                // Fill energy histograms
                // fillEnergyHistograms(event, *event.GetExperiment(), hist_ptr_map);

                // Fill addback histograms
                // fillAddbackHistograms(event, *event.GetExperiment(), hist_ptr_map);
            }
        }
    }

    void fillEnergyHistograms(Event &event, Experiment &experiment, HistogramManager::HistogramPtrMap &hist_ptr_map)
    {
    }

    void fillAddbackHistograms(Event &event, Experiment &experiment, HistogramManager::HistogramPtrMap &hist_ptr_map)
    {
    }

    void sortRun(Experiment &experiment, Run &run, HistogramManager &hist_man)
    {
        // Setup
        ROOT::EnableImplicitMT(std::thread::hardware_concurrency() - 2);

        // Setup the TTreeProcessorMT to process the run's tree
        ROOT::TTreeProcessorMT tree_processor(run.GetFileName(), run.GetTree()->GetName());

        // Setup the progress bar and timer
        std::atomic<ULong64_t> processedEntries(0);
        TStopwatch timer;

        auto sortTask = [&](TTreeReader &event_reader)
        {
            // Initialize
            auto hist_ptr_map = hist_man.MakeHistPtrMap();
            Event event(&experiment, &event_reader);

            // Event Loop
            while (event_reader.Next())
            {
                fillRawDataHistograms(event, hist_ptr_map);
                processedEntries++;
            }

            // Finalize
        };

        // Process the tree
        std::cout << "CloverSort [INFO]: Processing run " << run.GetRunNumber() << " with " << run.GetTree()->GetEntries() << " entries." << std::endl;
        timer.Start();
        std::thread progressbar_thread(Utilities::displayProgressBar, std::ref(processedEntries), run.GetTree()->GetEntries());
        tree_processor.Process(sortTask);

        // Finish processing
        timer.Stop();
        progressbar_thread.join();
        std::cout << "Processed events in " << timer.RealTime() << " seconds ("
                  << static_cast<double>(processedEntries) / timer.RealTime()
                  << " events/second)" << std::endl;

        // Merge the histograms
        // for (const auto &hist_ptr : hist_man.GetHistograms())
        // {
        //     hist_ptr->Merge();
        // }
    }

} // namespace Sort
