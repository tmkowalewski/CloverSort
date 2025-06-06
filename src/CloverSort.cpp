#include <iostream>

#include <TString.h>

#include "Experiment.hpp"
#include "Run.hpp"
#include "HistogramManager.hpp"
#include <ROOT/TTreeProcessorMT.hxx>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }

    try
    {
        // Extract experiment name from the config file name (without extension)
        TString config_path = argv[1];
        Ssiz_t lastSlash = config_path.Last('/');
        TString filename = (lastSlash != kNPOS) ? config_path(lastSlash + 1, config_path.Length() - lastSlash - 1) : config_path;
        Ssiz_t dotPos = filename.Last('.');
        TString experiment_name = (dotPos != kNPOS) ? filename(0, dotPos) : filename;

        Experiment experiment = Experiment(experiment_name, config_path);

        std::cout << "CloverSort [INFO]: Experiment " << experiment.getName() << " loaded successfully." << std::endl;
        std::cout << "CloverSort [INFO]: Tree named " << experiment.getRun(1)->getTree()->GetName() << " with " << experiment.getRun(1)->getTree()->GetEntries() << " entries found." << std::endl;

        ROOT::TTreeProcessorMT EventProcessor(experiment.getRun(1)->getFileName(), TString(experiment.getRun(1)->getTree()->GetName()));

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "CloverSort [ERROR]: " << e.what() << std::endl;
        return 1;
    }
}
