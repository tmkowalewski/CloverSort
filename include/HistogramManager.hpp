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
    HistogramManager() = default;
    ~HistogramManager();

    std::map<Detector *, std::vector<ROOT::TThreadedObject<TH1D> *>> getHistograms() const;

    void addHistogram(TString name, TH1D *histogram);
    void removeHistogram(TString name);

    void writeHistsToFile(TFile *file) const;
    void readHistsFromFile(TFile *file);

private:
    std::map<Detector *, std::vector<ROOT::TThreadedObject<TH1D> *>> histograms_; // Map to store histograms according to the Detector they belong to
};

#endif // HISTOGRAM_MANAGER_HPP