#ifndef RUN_HPP
#define RUN_HPP

// Standard C++ includes

// ROOT includes
#include <TFile.h>
#include <TTree.h>

// Project includes

// Forward declarations
class HistogramManager;

class Run
{

public:
    // Default Constructor
    Run(UInt_t run_number, const std::string &file_name, const std::string &tree_name, const std::string &run_description, const std::string &run_type);

    // Getters

    const UInt_t GetRunNumber() const { return run_number_; }
    const std::string &GetDescription() const { return run_description_; }
    const TFile *GetFile() const { return pfile_; }
    const std::string &GetFileName() const { return file_name_; }
    TTree *GetTree() const { return ptree_; }
    const TFile *GetHistFile() const { return phist_file_; }

    // Setters

    void SetHistManager(HistogramManager *phist_manager);
    void SetHistFile(TFile *phist_file);
    void SetHistFile(const std::string &hist_file_name);

    // Methods
    void CreateHistogramManager();

    void PrintInfo() const;

    // Destructor
    virtual ~Run();

private:
    const UInt_t run_number_;                   // Run number, unique identifier for the run
    const std::string run_description_;         // Name of the run, can be empty if not specified
    const std::string run_type_;                // Type of the run, can be empty if not specified
    const std::string file_name_;               // Name of the file associated with this run
    const std::string tree_name_;               // Name of the TTree associated with this run as defined by MVME
    std::string hist_file_name_;                // Name of the histogram file associated with this run
    TFile *pfile_ = nullptr;                    // Pointer to the ROOT file associated with this run
    TTree *ptree_ = nullptr;                    // Pointer to the ROOT tree associated with this run
    TFile *phist_file_ = nullptr;               // Pointer to the ROOT file for histograms, if applicable
    HistogramManager *phist_manager_ = nullptr; // Pointer to the HistogramManager for this run
};

#endif // RUN_HPP