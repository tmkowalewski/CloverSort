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
    Experiment(const TString name, const TString filename);
    ~Experiment();

    TString getName() const;

    void defineFromFile(const TString &filename);

    void addDAQModule(const DAQModule *module);
    void removeDAQModule(const TString &moduleName);
    const std::vector<DAQModule> getDAQModules() const;
    DAQModule *getDAQModule(const TString &moduleName) const;

    void addRun(const Run *run);
    void removeRun(const TString &runName);

    std::vector<Run> getRuns() const;
    Run *getRun(const Int_t runNumber) const;

private:
    TString name_;                        // Name of the experiment, same name as the ROOT Tree MVME generates
    TString filename_;                    // Name of the file where the experiment configuration is stored
    std::vector<DAQModule *> daqmodules_; // List of pointers to modules associated with the experiment
    std::vector<Run *> runs_;             // List of pointers to runs associated with the experiment
};

#endif // EXPERIMENT_HPP