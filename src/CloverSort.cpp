#include <iostream>

#include <TString.h>

#include "Experiment.hpp"
#include "Run.hpp"
#include "ROOT/TTreeProcessorMT.hxx"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }

    try
    {
        // Define the experiment from the configuration file
        std::cout << "CloverSort [INFO]: Initializing Experiment from configuration file: " << argv[1] << std::endl;
        Experiment Expt = Experiment(argv[1]);

        std::cout << "CloverSort [INFO]: Experiment " << Expt.getName() << " loaded successfully." << std::endl;
        std::cout << "CloverSort [INFO]: Tree named " << Expt.getRun(1)->getTree()->GetName() << " with " << Expt.getRun(1)->getTree()->GetEntries() << " entries found." << std::endl;

        ROOT::TTreeProcessorMT EventProcessor(Expt.getRun(1)->getFileName(), TString(Expt.getRun(1)->getTree()->GetName()));

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "CloverSort [ERROR]: " << e.what() << std::endl;
        return 1;
    }
}
