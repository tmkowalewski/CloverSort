#ifndef EXPERIMENT_HPP
#define EXPERIMENT_HPP

#include <vector>
#include <TString.h>

// Forward declarations

class DAQModule;
class Run;

class Experiment
{
public:
    Experiment(const TString name, const TString file_name);
    ~Experiment();

    // Getters

    const TString &getName() const;
    const DAQModule *getDAQModule(const TString module_name) const;
    const std::vector<DAQModule> *getDAQModules() const;
    const Run *getRun(const Int_t runNumber) const;
    const std::vector<Run> *getRuns() const;

    // Setters

    // Methods
    void addDAQModule(const DAQModule &module);
    void removeDAQModule(const TString &module_name);
    void addRun(const Run &run);
    void removeRun(const Int_t &run_name);

    void printInfo() const;

private:
    TString name_;                       // Name of the experiment, same name as the ROOT Tree MVME generates
    TString file_name_;                  // Name of the file where the experiment configuration is stored
    std::vector<DAQModule> daq_modules_; // List of pointers to modules associated with the experiment
    std::vector<Run> runs_;              // List of pointers to runs associated with the experiment
};

std::vector<Int_t> parseNumberString(const TString &number_string);

#endif // EXPERIMENT_HPP