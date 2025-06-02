#include "Run.hpp"

Run::Run(Int_t runNumber, TString fileName, TString treeName, TString runName, TString runType)
    : runNumber_(runNumber), filename_(fileName), treeName_(treeName), runName_(runName), runType_(runType)
{
    file_ = TFile::Open(filename_);
    if (!file_ || file_->IsZombie())
    {
        throw std::runtime_error("Error opening file: " + filename_);
    }
    tree_ = static_cast<TTree *>(file_->Get(treeName_));
    if (!tree_)
    {
        throw std::runtime_error("Error retrieving TTree" + treeName_ + " from file " + filename_);
    }
}