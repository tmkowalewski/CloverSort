#ifndef HISTOGRAM_MANAGER_HPP
#define HISTOGRAM_MANAGER_HPP

#include <vector>
#include <map>
#include <TH1D.h>
#include <ROOT/TThreadedObject.hxx>

// Forward declarations

class DAQModule;
class Detector;

class HistogramManager
{
public:
    HistogramManager();
    ~HistogramManager();

    // Getters
    using HistogramMap = std::map<std::vector<TString>, std::vector<ROOT::TThreadedObject<TH1D> *>>;
    const HistogramMap &getHistograms() const { return histogram_map_; }

    // Setters

    // Methods

    void addHistogram(const TString &name, const TString &title, const Int_t nbinsx, const Double_t &xlow, const Double_t &xup, std::vector<TString> &histogram_path);
    void removeHistogram(const std::vector<TString> &histogram_path, const TString &name);

    using HistogramPtrMap = std::map<std::vector<TString>, std::vector<std::shared_ptr<TH1D>>>;
    HistogramPtrMap makeHistPtrMap();

    void writeHistsToFile(TFile *file);
    // void readHistsFromFile(TFile *file);

    void printInfo();

private:
    HistogramMap histogram_map_; // Map of histograms managed by this class
};

#endif // HISTOGRAM_MANAGER_HPP