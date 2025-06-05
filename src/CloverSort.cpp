#include <iostream>

#include <TString.h>

#include "Experiment.hpp"
#include "DAQModule.hpp"
#include "Detector.hpp"
#include "Run.hpp"
#include "HistogramManager.hpp"

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

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
