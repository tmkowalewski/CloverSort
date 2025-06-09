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
        // Define the experiment from the configuration file
        std::cout << "CloverSort [INFO]: Initializing Experiment from configuration file: " << argv[1] << std::endl;
        Experiment Expt = Experiment(argv[1]);

        std::cout << "CloverSort [INFO]: Experiment " << Expt.getName() << " loaded successfully." << std::endl;
        std::cout << "CloverSort [INFO]: Tree named " << Expt.getRun(1)->getTree()->GetName() << " with " << Expt.getRun(1)->getTree()->GetEntries() << " entries found." << std::endl;

        ROOT::TTreeProcessorMT EventProcessor(Expt.getRun(1)->getFileName(), TString(Expt.getRun(1)->getTree()->GetName()));

        TaskManager task_manager;

        Task<void(int), char(), bool(int)> task("test", init, execute, final);
        task.setInitializeArguments(std::make_tuple(1));
        task.setExecuteArguments(std::make_tuple());
        task.setFinalizeArguments(std::make_tuple(1));

        task_manager.addTask(&task);
        task_manager.initializeTasks();
        task_manager.executeTasks();
        task_manager.finalizeTasks();

        std::cout << (task.getExecuteOutput() == '!' ? Form("CloverSort [INFO]: Task %s executed successfully.", task.getName().Data()) : Form("CloverSort [ERROR]: Task %s execution failed.", task.getName().Data())) << std::endl;

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "CloverSort [ERROR]: " << e.what() << std::endl;
        return 1;
    }
}
