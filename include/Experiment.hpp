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
    // Constructors
    Experiment(const TString file_name);
    Experiment();

    // Default destructor method
    ~Experiment();

    // Getters

    const TString &GetName() const { return name_; }
    const DAQModule *GetDAQModule(const TString &module_name) const;
    const std::vector<DAQModule *> &GetDAQModules() const { return daq_modules_; }
    const Run *GetRun(const Int_t runNumber) const;
    const std::vector<Run *> &GetRuns() const { return runs_; }

    // Setters

    // Methods
    void AddDAQModule(DAQModule *module);
    void RemoveDAQModule(const TString &module_name);
    void AddRun(Run *run);
    void RemoveRun(const Int_t run_number);

    void PrintInfo() const;

private:
    TString name_;                         // Name of the experiment, same name as the ROOT Tree MVME generates
    TString file_name_;                    // Name of the file where the experiment configuration is stored
    std::vector<DAQModule *> daq_modules_; // List of pointers to modules associated with the experiment
    std::vector<Run *> runs_;              // List of pointers to runs associated with the experiment
};

std::vector<Int_t> parseNumberString(const TString &number_string);

#endif // EXPERIMENT_HPP