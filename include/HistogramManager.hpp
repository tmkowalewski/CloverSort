#ifndef HISTOGRAM_MANAGER_HPP
#define HISTOGRAM_MANAGER_HPP

#include <vector>
#include <map>
#include <TH1D.h>
#include <ROOT/TThreadedObject.hxx>

// Forward declarations

class Experiment;
class DAQModule;
class Detector;

class HistogramManager
{
public:
    using HistogramMap = std::map<std::vector<TString>, std::vector<ROOT::TThreadedObject<TH1D> *>>;
    using HistogramPtrMap = std::map<std::vector<TString>, std::vector<std::shared_ptr<TH1D>>>;

    // Constructors

    HistogramManager();
    HistogramManager(Experiment *experiment);

    // Default destructor method
    ~HistogramManager();

    // Getters
    const HistogramMap &getHistograms() const { return histogram_map_; }
    const TString &GetName() const { return name_; }

    // Setters

    void SetName(const TString &name) { name_ = name; }

    // Methods

    void InitFromExperiment(Experiment *experiment);

    void AddHistogram(const TString &name, const TString &title, const Int_t nbinsx, const Double_t &xlow, const Double_t &xup, std::vector<TString> &histogram_path);
    void RemoveHistogram(const std::vector<TString> &histogram_path, const TString &name);

    HistogramPtrMap MakeHistPtrMap();

    void WriteHistsToFile(TFile *file);

    void PrintInfo();

private:
    TString name_;
    HistogramMap histogram_map_; // Map of histograms managed by this class
};

#endif // HISTOGRAM_MANAGER_HPP