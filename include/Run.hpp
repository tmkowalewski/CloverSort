#ifndef RUN_HPP
#define RUN_HPP

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include "HistogramManager.hpp"

class Run
{

public:
    // Default Constructor
    Run(Int_t run_number, TString file_name, TString tree_name, TString run_description, TString run_type);

    // Getters

    const Int_t GetRunNumber() const { return run_number_; }
    const TString &GetDescription() const { return run_description_; }
    const TFile *GetFile() const { return pfile_; }
    const TString &GetFileName() const { return file_name_; }
    const TTree *GetTree() const { return ptree_; }
    const TFile *GetHistFile() const { return phist_file_; }

    // Setters

    void SetRunNumber(const int run_number) { run_number_ = run_number; }
    void SetFile(TFile *file);
    void SetFile(const TString &file_name);
    void SetTree(TTree *tree);

    void SetHistFile(TFile *phist_file);
    void SetHistFile(const TString &hist_file_name);

    // Methods
    void CreateHistogramManager();

    void PrintInfo() const;

    // Destructor
    virtual ~Run();

private:
    Int_t run_number_;            // Run number, unique identifier for the run
    TString run_description_;     // Name of the run, can be empty if not specified
    TString run_type_;            // Type of the run, can be empty if not specified
    TString file_name_;           // Name of the file associated with this run
    TString tree_name_;           // Name of the TTree associated with this run as defined by MVME
    TString hist_file_name_;      // Name of the histogram file associated with this run
    TFile *pfile_ = nullptr;      // Pointer to the ROOT file associated with this run
    TTree *ptree_ = nullptr;      // Pointer to the ROOT tree associated with this run
    TFile *phist_file_ = nullptr; // Pointer to the ROOT file for histograms, if applicable
};

#endif // RUN_HPP