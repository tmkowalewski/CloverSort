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
    // Constructors

    HistogramManager();
    HistogramManager(Experiment *experiment);

    // Default destructor method
    ~HistogramManager();

    // Getters
    using HistogramMap = std::map<std::vector<TString>, std::vector<ROOT::TThreadedObject<TH1D> *>>;
    const HistogramMap &getHistograms() const { return histogram_map_; }
    const TString &getName() const { return name_; }

    // Setters

    void setName(const TString &name) { name_ = name; }

    // Methods

    void initFromExperiment(Experiment *experiment);

    void addHistogram(const TString &name, const TString &title, const Int_t nbinsx, const Double_t &xlow, const Double_t &xup, std::vector<TString> &histogram_path);
    void removeHistogram(const std::vector<TString> &histogram_path, const TString &name);

    using HistogramPtrMap = std::map<std::vector<TString>, std::vector<std::shared_ptr<TH1D>>>;
    HistogramPtrMap makeHistPtrMap();

    void writeHistsToFile(TFile *file);

    void printInfo();

private:
    TString name_;
    HistogramMap histogram_map_; // Map of histograms managed by this class
};

#endif // HISTOGRAM_MANAGER_HPP