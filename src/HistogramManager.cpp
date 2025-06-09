#include "HistogramManager.hpp"

HistogramManager::HistogramManager()
{
    // Constructor implementation
}

HistogramManager::~HistogramManager()
{
}

void HistogramManager::addHistogram(const TString &name, const TString &title, const Int_t nbinsx, const Double_t &xlow, const Double_t &xup, std::vector<TString> &histogram_path)
{
    auto hist = new ROOT::TThreadedObject<TH1D>(name, title, nbinsx, xlow, xup);
    histogram_map_[histogram_path].push_back(hist);
}

void HistogramManager::removeHistogram(const std::vector<TString> &histogram_path, const TString &name)
{
    std::remove_if(histogram_map_[histogram_path].begin(), histogram_map_[histogram_path].end(),
                   [&name](ROOT::TThreadedObject<TH1D> *hist)
                   { return hist->Get()->GetName() == name; });
}

HistogramManager::HistogramPtrMap HistogramManager::makeHistPtrMap()
{
    HistogramPtrMap hist_ptr_map;
    for (auto &pair : histogram_map_)
    {
        const auto &histogram_path = pair.first;
        auto &histograms = pair.second;
        std::vector<std::shared_ptr<TH1D>> hist_ptrs;
        for (auto &histogram : histograms)
        {
            hist_ptrs.push_back(histogram->Get());
        }
        hist_ptr_map[histogram_path] = hist_ptrs;
    }
    return hist_ptr_map;
}
