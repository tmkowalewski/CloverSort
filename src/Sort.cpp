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
#include "Event.hpp"
#include "Run.hpp"
#include "HistogramManager.hpp"
#include "Utilities.hpp"

namespace Sort
{

    void fillRawDataHistograms(Event &event, Experiment &experiment, HistogramManager &hist_man)
    {
        auto hist_ptr_map = hist_man.MakeHistPtrMap();
        for (const auto &pdaq_module : experiment.GetDAQModules())
        {
            for (const auto &filter : pdaq_module->GetFilters())
            {
                if ((std::string)filter == "module_timestamp" || (std::string)filter == "trigger_time")
                {
                }
            }
        }
    }

    void sortRun(Experiment &experiment, Run &run)
    {

        // Setup the TTreeProcessorMT to process the run's tree
        ROOT::TTreeProcessorMT tree_processor(run.GetFileName(), run.GetTree()->GetName());

        // Setup the progress bar and timer
        std::atomic<ULong64_t> processedEntries(0);
        TStopwatch timer;

        auto sortTask = [&](TTreeReader &event_reader)
        {
            // Initialize

            // Event Loop

            // Finalize

            processedEntries++;
        };

        // Process the tree
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
