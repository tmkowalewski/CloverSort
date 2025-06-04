#ifndef RUN_HPP
#define RUN_HPP

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include "HistogramManager.hpp"

class Run
{

    friend class Experiment; // Allow Experiment to access private members

public:
    // Default Constructor

    // Getters

    const Int_t getRunNumber() const;
    const TString &getDescription() const;
    const TFile *getFile() const;
    const TString &getFileName() const;
    const TTree *getTree() const;
    const TFile *getHistFile() const;
    const auto *getHistMan() const;

    // Setters

    void setRunNumber(int run_number);
    void setFile(TFile *file);
    void setFile(TString file_name);
    void setTree(TTree *tree);

    void setHistFile(TFile *phist_file);
    void setHistFile(TString hist_file_name);

    // Methods
    void createHistogramManager();

    void printInfo() const;

    // Destructor
    virtual ~Run();

private:
    Run(Int_t run_number, TString file_name, TString tree_name, TString run_description, TString run_type); // Private constructor to prevent instantiation without an Experiment context

    Int_t run_number_;              // Run number, unique identifier for the run
    TString run_description_;       // Name of the run, can be empty if not specified
    TString run_type_;              // Type of the run, can be empty if not specified
    TString file_name_;             // Name of the file associated with this run
    TString tree_name_;             // Name of the TTree associated with this run as defined by MVME
    TString hist_file_name_;        // Name of the histogram file associated with this run
    TFile *pfile_;                  // Pointer to the ROOT file associated with this run
    TTree *ptree_;                  // Pointer to the ROOT tree associated with this run
    TFile *phist_file_;             // Pointer to the ROOT file for histograms, if applicable
    HistogramManager hist_manager_; // The HistogramManager for this run
};

#endif // RUN_HPP