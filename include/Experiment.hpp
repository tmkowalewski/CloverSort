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

    const TString &getName() const { return name_; }
    const DAQModule *getDAQModule(const TString &module_name) const;
    const std::vector<DAQModule *> *getDAQModules() const { return &daq_modules_; }
    const Run *getRun(const Int_t runNumber) const;
    const std::vector<Run *> *getRuns() const { return &runs_; }

    // Setters

    // Methods
    void addDAQModule(DAQModule *module);
    void removeDAQModule(const TString &module_name);
    void addRun(Run *run);
    void removeRun(const Int_t run_number);

    void printInfo() const;

private:
    TString name_;                         // Name of the experiment, same name as the ROOT Tree MVME generates
    TString file_name_;                    // Name of the file where the experiment configuration is stored
    std::vector<DAQModule *> daq_modules_; // List of pointers to modules associated with the experiment
    std::vector<Run *> runs_;              // List of pointers to runs associated with the experiment
};

std::vector<Int_t> parseNumberString(const TString &number_string);

#endif // EXPERIMENT_HPP