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

    void fillRawDataHistograms(Event &event, HistogramManager &hist_man)
    {
    }

    void fillEnergyHistograms(Event &event, Experiment &experiment, HistogramManager &hist_man)
    {
    }

    void fillAddbackHistograms(Event &event, Experiment &experiment, HistogramManager &hist_man)
    {
    }

    void sortRun(Experiment &experiment, Run &run)
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
            Event event(&experiment, &event_reader);

            // Event Loop
            while (event_reader.Next())
            {
                processedEntries++;
            }

            // Finalize
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
