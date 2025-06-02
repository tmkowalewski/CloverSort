#ifndef RUN_HPP
#define RUN_HPP

#include <TString.h>
#include <TFile.h>
#include <TTree.h>

// Forward declarations

class HistogramManager;

class Run
{
public:
    // Default Constructor
    Run(Int_t runNumber, TString fileName, TString treeName, TString name = "");

    // Getters

    Int_t getRunNumber() const;
    TString getName() const;
    TFile *getFile() const;
    TString getFileName() const;
    TTree *getTree() const;
    TFile *getHistFile() const;

    // Setters

    void setRunNumber(int runNumber);
    void setFile(TFile *file);
    void setFile(TString fileName);
    void setTree(TTree *tree);

    void setHistFile(TFile *histFile);

    // Destructor
    virtual ~Run();

private:
    Int_t runNumber_;               // Run number, unique identifier for the run
    TString runName_;               // Name of the run, can be empty if not specified
    TString runtype_;               // Type of the run, can be empty if not specified
    TString filename_;              // Name of the file associated with this run
    TString treeName_;              // Name of the TTree associated with this run as defined by MVME
    TFile *file_;                   // Pointer to the ROOT file associated with this run
    TTree *tree_;                   // Pointer to the ROOT tree associated with this run
    TFile *histFile_;               // Pointer to the ROOT file for histograms, if applicable
    HistogramManager *histManager_; // Pointer to the HistogramManager for this run
};

#endif // RUN_HPP