// Standard C++ includes
#include <iostream>

// ROOT includes
#include <TString.h>

// Project includes
#include "Experiment.hpp"
#include "Run.hpp"
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

        for (const auto &run : Expt.getRuns())
        {
            ROOT::TTreeProcessorMT tree_processor(run->getFileName(), run->getTree()->GetName());
            HistogramManager hist_manager(&Expt);
            hist_manager.printInfo();
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "CloverSort [ERROR]: " << e.what() << std::endl;
        return 1;
    }
}
