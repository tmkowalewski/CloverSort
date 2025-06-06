#include <iostream>

#include <TString.h>

#include "Experiment.hpp"
#include "Run.hpp"
#include "TaskManager.hpp"
#include "Task.hpp"
#include <ROOT/TTreeProcessorMT.hxx>

void init(int i)
{
    std::cout << "CloverSort [INFO]: Initialize Task " << i << std::endl;
}

char execute()
{
    std::cout << "CloverSort [INFO]: Execute Task " << '!' << std::endl;
    return '!';
}

bool final(int i)
{
    std::cout << "CloverSort [INFO]: Finalize Task " << (i == 1) << std::endl;
    return bool(i == 1);
}

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

        Experiment Expt = Experiment(experiment_name, config_path);

        std::cout << "CloverSort [INFO]: Experiment " << Expt.getName() << " loaded successfully." << std::endl;
        std::cout << "CloverSort [INFO]: Tree named " << Expt.getRun(1)->getTree()->GetName() << " with " << Expt.getRun(1)->getTree()->GetEntries() << " entries found." << std::endl;

        ROOT::TTreeProcessorMT EventProcessor(Expt.getRun(1)->getFileName(), TString(Expt.getRun(1)->getTree()->GetName()));

        TaskManager task_manager;
        Task<void(int), char(), bool(int)> task(init, execute, final);
        task.callInitialize(1);
        std::cout << task.callExecute() << std::endl;
        std::cout << task.callFinalize(1) << std::endl;

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "CloverSort [ERROR]: " << e.what() << std::endl;
        return 1;
    }
}
