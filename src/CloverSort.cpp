// Standard C++ includes
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

// ROOT includes
#include <TString.h>
#include <TStopwatch.h>

// Project includes
#include "Experiment.hpp"
#include "Run.hpp"
#include "Event.hpp"
#include "HistogramManager.hpp"
#include <ROOT/TTreeProcessorMT.hxx>

int main(int argc, char *argv[])
{
    try
    {
        if (argc < 2)
        {
            throw std::runtime_error(Form("No configuration file provided. Usage: %s /path/to/config.conf", argv[0]));
            return 1;
        }

        // Define the experiment from the configuration file
        Experiment Expt = Experiment(argv[1]);
        std::cout << "CloverSort [INFO]: Defined Experiment " << Expt.GetName() << " from configuration file " << Expt.GetFileName() << "!" << std::endl;

        for (const auto &run : Expt.GetRuns())
        {
            ROOT::TTreeProcessorMT tree_processor(run->GetFileName(), run->GetTree()->GetName());
            HistogramManager hist_manager(&Expt);
            // hist_manager.PrintInfo();
            auto hist_ptr_map = hist_manager.MakeHistPtrMap();

            TTreeReader tree_reader(run->GetTree());

            TStopwatch timer;
            timer.Start();
            Event event(&Expt, &tree_reader);
            std::cout << event.GetData("clover_cross", "amplitude", 0) << std::endl;
            timer.Stop();
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "CloverSort [ERROR]: " << e.what() << std::endl;
        return 1;
    }
}
