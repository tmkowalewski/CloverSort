// Standard C++ includes
#include <iostream>

// ROOT includes

// Project includes
#include "Run.hpp"
#include "HistogramManager.hpp"

Run::Run(UInt_t run_number, const std::string &run_description, const std::string &run_type, const std::string &file_name, const std::string &tree_name)
    : run_number_(run_number), run_description_(run_description), run_type_(run_type), file_name_(file_name), tree_name_(tree_name)
{
    // Initialize file and tree pointers
    // Open the ROOT file (read only) and retrieve the TTree
    pfile_ = TFile::Open(file_name.c_str(), "READ");
    if (!pfile_ || pfile_->IsZombie())
    {
        throw std::runtime_error(Form("Error opening file: %s", file_name_.c_str()));
    }

    ptree_ = static_cast<TTree *>(pfile_->Get(tree_name_.c_str()));
    if (!ptree_)
    {
        throw std::runtime_error(Form("Error retrieving TTree %s from file %s", tree_name_.c_str(), file_name_.c_str()));
    }

    // Initialize histogram file pointer
    phist_file_ = nullptr; // Initially set to nullptr, can be set later if needed
}

Run::~Run()
{
    // Clean up dynamically allocated memory
    if (pfile_)
    {
        pfile_->Close();
        delete pfile_;
    }
    if (phist_file_)
    {
        phist_file_->Close();
        delete phist_file_;
    }
    if (ptree_)
    {
        delete ptree_;
    }
}

void Run::SetHistManager(HistogramManager *phist_manager)
{
    if (phist_manager_)
    {
        delete phist_manager_;
    }
    phist_manager_ = phist_manager;

    if (!phist_manager_)
    {
        throw std::runtime_error("Error setting HistogramManager");
    }
}

void Run::SetHistFile(TFile *histFile)
{
    if (phist_file_)
    {
        phist_file_->Close();
        delete phist_file_;
    }
    phist_file_ = histFile;

    if (!phist_file_ || phist_file_->IsZombie())
    {
        throw std::runtime_error(Form("Error setting histogram file: %s", hist_file_name_.c_str()));
    }

    hist_file_name_ = phist_file_->GetName(); // Update histFileName_ to match the new histogram file
}

void Run::SetHistFile(const std::string &histFileName)
{
    if (phist_file_)
    {
        phist_file_->Close();
        delete phist_file_;
    }
    phist_file_ = TFile::Open(histFileName.c_str(), "UPDATE");
    if (!phist_file_ || phist_file_->IsZombie())
    {
        throw std::runtime_error(Form("Error opening histogram file: %s", histFileName.c_str()));
    }
    hist_file_name_ = histFileName;
}

void Run::PrintInfo() const
{
    TString short_file_name = file_name_;
    if (short_file_name.Contains("/"))
        short_file_name = short_file_name.Tokenize("/")->Last()->GetName();
    std::cout << Form("%i (%s) (%s) [%s]", run_number_, run_description_.c_str(), run_type_.c_str(), short_file_name.Data()) << std::endl;
}