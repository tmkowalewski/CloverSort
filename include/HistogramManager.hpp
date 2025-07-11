#ifndef HISTOGRAM_MANAGER_HPP
#define HISTOGRAM_MANAGER_HPP

// Standard C++ includes
#include <vector>
#include <string>
#include <unordered_map>

// ROOT includes
#include <TH1D.h>
#include <ROOT/TThreadedObject.hxx>

// Project includes

// Forward declarations
class Experiment;
class DAQModule;
class Detector;

struct HistKey
{
    std::string module;
    std::string detector;
    std::string filter;
    UInt_t index;

    bool operator==(const HistKey &other) const
    {
        return std::tie(module, detector, filter, index) ==
               std::tie(other.module, other.detector, other.filter, other.index);
    }
};

class HistogramManager
{
public:
    // Typedefs

    typedef std::unordered_map<std::string,
                               std::unordered_map<std::string,
                                                  std::unordered_map<UInt_t, ROOT::TThreadedObject<TH1D> *>>>
        HistogramMap;

    typedef std::unordered_map<std::string,
                               std::unordered_map<std::string,
                                                  std::unordered_map<UInt_t, std::shared_ptr<TH1D>>>>
        HistogramPtrMap;

    // Constructors

    HistogramManager();
    HistogramManager(Experiment *experiment);

    // Default destructor method
    ~HistogramManager();

    // Getters
    const HistogramMap &GetHistograms() const { return histogram_map_; }
    const std::string &GetName() const { return name_; }

    // Setters

    // Methods

    void InitFromExperiment(Experiment *experiment);

    void AddHistogram(const std::string &name, const std::string &title, const Int_t nbinsx, const Double_t &xlow, const Double_t &xup, const std::string &owner, const std::string &filter, const UInt_t index = 0);

    HistogramPtrMap MakeHistPtrMap();

    void WriteHistsToFile(TFile *file);

    void PrintInfo();

private:
    const std::string name_;
    HistogramMap histogram_map_; // Map of histograms managed by this class
};

#endif // HISTOGRAM_MANAGER_HPP