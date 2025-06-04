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
    friend class Run; // Allow Run to access private members

public:
    ~HistogramManager();

    // Getters

    const std::map<TString, std::vector<ROOT::TThreadedObject<TH1D>>> *getHistograms() const;

    // Setters

    // Methods

    void addHistogram(Detector detector, ROOT::TThreadedObject<TH1D> *histogram);
    void removeHistogram(TString detector_name, TString name);

    void writeHistsToFile(TFile *file);
    // void readHistsFromFile(TFile *file);

    void printInfo();

private:
    HistogramManager(); // Private constructor to prevent instantiation without a Run context

    std::map<TString, std::vector<ROOT::TThreadedObject<TH1D>>> histograms_; // Map to store histograms according to the Detector they belong to
};

#endif // HISTOGRAM_MANAGER_HPP