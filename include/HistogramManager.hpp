#ifndef HISTOGRAM_MANAGER_HPP
#define HISTOGRAM_MANAGER_HPP

#include <vector>
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

    const std::map<TString, std::vector<ROOT::TThreadedObject<TH1D>>> *getHistograms() const { return &histogram_map_; }

    // Setters

    // Methods

    void addHistogram(ROOT::TThreadedObject<TH1D> *histogram);
    void removeHistogram(const TString &detector_name, const TString &name);
    std::map<std::vector<TString>, std::vector<std::shared_ptr<TH1D>>> generateHistPtrMap() const;

    void writeHistsToFile(TFile *file);
    // void readHistsFromFile(TFile *file);

    void printInfo();

private:
    std::map<TString, std::vector<ROOT::TThreadedObject<TH1D>>> histogram_map_; // Map of histograms managed by this class
};

#endif // HISTOGRAM_MANAGER_HPP