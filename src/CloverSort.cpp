// Standard C++ includes
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

// ROOT includes

// Project includes
#include "Experiment.hpp"
#include "Run.hpp"
#include "Event.hpp"
#include "HistogramManager.hpp"
#include "Sort.hpp"

int main(int argc, char *argv[])
{
    try
    {
        if (argc < 2)
        {
            throw std::runtime_error(Form("No configuration file provided. Usage: %s /path/to/config.conf", argv[0]));
        }

        // Define the experiment from the configuration file
        Experiment expt = Experiment(argv[1]);

        for (auto run : expt.GetRuns())
        {
            HistogramManager hist_manager(&expt);
            hist_manager.PrintInfo();

            Sort::sortRun(expt, *run, hist_manager, "raw");

            hist_manager.MergeHistograms();
            hist_manager.WriteHistsToFile(Form("run_%i.root", run->GetRunNumber()));
            std::cout << "CloverSort [INFO]: Finished processing run " << run->GetRunNumber() << std::endl;
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "CloverSort [ERROR]: " << e.what() << std::endl;
        return 1;
    }
}