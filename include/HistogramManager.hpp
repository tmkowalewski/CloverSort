#ifndef HISTOGRAM_MANAGER_HPP
#define HISTOGRAM_MANAGER_HPP

#include <map>
#include <TH1D.h>
#include <TStopwatch.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <ROOT/TThreadedObject.hxx>
#include <ROOT/TTreeProcessorMT.hxx>
#include <TFile.h>

// Forward declarations

class DAQModule;
class Detector;

class HistogramManager
{
public:
    HistogramManager();
    ~HistogramManager();

    // Getters

    const std::map<TString, ROOT::TThreadedObject<TH1D>> *getHistograms() const;

    // Setters

    // Methods

    void addHistogram(ROOT::TThreadedObject<TH1D> &histogram);
    void removeHistogram(TString detector_name, TString name);
    std::map<std::vector<TString>, std::vector<std::shared_ptr<TH1D>>> generateHistPtrMap() const;

    void writeHistsToFile(TFile *file);
    // void readHistsFromFile(TFile *file);

    void printInfo();

private:
    std::map<std::vector<TString>, std::vector<ROOT::TThreadedObject<TH1D>>> histogram_map_; // Map of histograms managed by this class
};

#endif // HISTOGRAM_MANAGER_HPP