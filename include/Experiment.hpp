#ifndef EXPERIMENT_HPP
#define EXPERIMENT_HPP

// Standard C++ includes
#include <string>
#include <vector>

// ROOT includes

// Project includes

// Forward declarations
class DAQModule;
class Run;

class Experiment
{
public:
    // Constructors
    Experiment(const std::string &file_name);
    Experiment();

    // Default destructor method
    ~Experiment();

    // Getters

    const std::string &GetName() const { return name_; }
    const std::string &GetFileName() const { return file_name_; }
    const DAQModule *GetDAQModule(const std::string &module_name) const;
    const std::vector<DAQModule *> &GetDAQModules() const { return daq_modules_; }
    const Run *GetRun(const UInt_t runNumber) const;
    const std::vector<Run *> &GetRuns() const { return runs_; }

    // Setters

    // Methods
    void AddDAQModule(DAQModule *module);
    void RemoveDAQModule(const std::string &module_name);
    void AddRun(Run *run);
    void RemoveRun(const UInt_t run_number);

    void PrintInfo() const;

private:
    std::string name_;                     // Name of the experiment, same name as the ROOT Tree MVME generates
    std::string file_name_;                // Name of the file where the experiment configuration is stored
    std::vector<DAQModule *> daq_modules_; // List of pointers to modules associated with the experiment
    std::vector<Run *> runs_;              // List of pointers to runs associated with the experiment
};

#endif // EXPERIMENT_HPP