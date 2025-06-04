#include "Run.hpp"
#include "HistogramManager.hpp"

Run::Run(Int_t run_number, TString file_name, TString tree_name, TString run_description, TString run_type)
    : run_number_(run_number), file_name_(file_name), tree_name_(tree_name), run_description_(run_description), run_type_(run_type)
{
    // Initialize file and tree pointers
    // Open the ROOT file (read only) and retrieve the TTree
    pfile_ = TFile::Open(file_name, "READ");
    if (!pfile_ || pfile_->IsZombie())
    {
        throw std::runtime_error("Error opening file: " + file_name);
    }

    ptree_ = static_cast<TTree *>(pfile_->Get(tree_name_));
    if (!ptree_)
    {
        throw std::runtime_error("Error retrieving TTree" + tree_name_ + " from file " + file_name);
    }

    // Initialize histogram file pointer
    phist_file_ = nullptr; // Initially set to nullptr, can be set later if needed

    // Initialize histogram manager pointer
    hist_manager_ = *(new HistogramManager()); // Assuming HistogramManager has a default constructor
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
    delete &hist_manager_;
}

const Int_t Run::getRunNumber() const
{
    return run_number_;
}

const TString &Run::getDescription() const
{
    return run_description_;
}

const TFile *Run::getFile() const
{
    return pfile_;
}

const TString &Run::getFileName() const
{
    return file_name_;
}

const TTree *Run::getTree() const
{
    return ptree_;
}

const TFile *Run::getHistFile() const
{
    return phist_file_;
}

void Run::setRunNumber(int runNumber)
{
    run_number_ = runNumber;
}

void Run::setFile(TFile *file)
{
    if (pfile_)
    {
        pfile_->Close();
        delete pfile_;
    }
    pfile_ = file;

    if (!pfile_ || pfile_->IsZombie())
    {
        throw std::runtime_error("Error setting file: " + file_name_);
    }
    file_name_ = pfile_->GetName(); // Update filename_ to match the new file
}

void Run::setFile(TString fileName)
{
    if (pfile_)
    {
        pfile_->Close();
        delete pfile_;
    }
    pfile_ = TFile::Open(fileName, "READ");
    if (!pfile_ || pfile_->IsZombie())
    {
        throw std::runtime_error("Error opening file: " + fileName);
    }
    fileName = fileName;
}

void Run::setTree(TTree *tree)
{
    if (ptree_)
    {
        delete ptree_;
    }
    ptree_ = tree;

    if (!ptree_)
    {
        throw std::runtime_error("Error setting TTree: " + tree_name_);
    }

    tree_name_ = ptree_->GetName(); // Update treeName_ to match the new tree
}

void Run::setHistFile(TFile *histFile)
{
    if (phist_file_)
    {
        phist_file_->Close();
        delete phist_file_;
    }
    phist_file_ = histFile;

    if (!phist_file_ || phist_file_->IsZombie())
    {
        throw std::runtime_error("Error setting histogram file: " + hist_file_name_);
    }

    hist_file_name_ = phist_file_->GetName(); // Update histFileName_ to match the new histogram file
}

void Run::setHistFile(TString histFileName)
{
    if (phist_file_)
    {
        phist_file_->Close();
        delete phist_file_;
    }
    phist_file_ = TFile::Open(histFileName, "UPDATE");
    if (!phist_file_ || phist_file_->IsZombie())
    {
        throw std::runtime_error("Error opening histogram file: " + histFileName);
    }
    hist_file_name_ = histFileName;
}
