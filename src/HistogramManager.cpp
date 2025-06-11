#include <iostream>
#include "HistogramManager.hpp"
#include "Experiment.hpp"
#include "DAQModule.hpp"
#include "Detector.hpp"

HistogramManager::HistogramManager()
{
    // Constructor implementation
}

HistogramManager::HistogramManager(Experiment *experiment)
{
    for (const auto &pdaq_module : experiment->getDAQModules())
    {
        for (const auto &filter : pdaq_module->getFilters())
        {
            if (filter == "module_timestamp") // Only one value of module_timestamp per module
            {
                std::vector<TString> histogram_path = {pdaq_module->getName()};
                TString hist_name = Form("%s.%s", pdaq_module->getName().Data(), filter.Data());
                TString hist_title = Form("%s %s", pdaq_module->getName().Data(), filter.Data());
                addHistogram(hist_name, hist_title, DAQ_BINS, 0, DAQ_BINS - 1, histogram_path);
                continue;
            }
            for (const auto &pdetector : pdaq_module->getDetectors())
            {
                for (const auto &channel : pdetector->getChannels())
                {
                    // Every filter on every channel of every detector in every daq module gets a histogram
                    std::vector<TString> histogram_path = {pdaq_module->getName(), pdetector->getName()};
                    Int_t crystal_num = channel % pdetector->getChannels().size() + 1;
                    TString hist_name = Form("%sE%i.%s", pdetector->getName().Data(), crystal_num, filter.Data());
                    TString hist_title = Form("%sE%i %s", pdetector->getName().Data(), crystal_num, filter.Data());
                    addHistogram(hist_name, hist_title, DAQ_BINS, 0, DAQ_BINS - 1, histogram_path);
                }
            }
        }
    }
}

HistogramManager::~HistogramManager()
{
    // Destructor implementation
    for (auto &pair : histogram_map_)
    {
        for (auto &histogram : pair.second)
        {
            delete histogram; // Clean up dynamically allocated histograms
        }
    }
    histogram_map_.clear(); // Clear the map
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

void HistogramManager::printInfo()
{
    std::cout << "HistogramManager Info:" << std::endl;
    size_t path_count = 0;
    for (const auto &pair : histogram_map_)
    {
        const auto &histogram_path = pair.first;
        const auto &histograms = pair.second;
        bool is_last_path = (++path_count == histogram_map_.size());

        std::cout << (is_last_path ? "└── " : "├── ") << "Path: ";
        for (size_t i = 0; i < histogram_path.size(); ++i)
        {
            std::cout << histogram_path[i] << (i == histogram_path.size() - 1 ? "" : "/");
        }
        std::cout << "/" << std::endl;

        size_t hist_count = 0;
        for (const auto &histogram : histograms)
        {
            bool is_last_hist = (++hist_count == histograms.size());
            std::cout << (is_last_path ? "    " : "│   ") << (is_last_hist ? "└── " : "├── ");
            std::cout << "Histogram: " << histogram->Get()->GetName() << " - " << histogram->Get()->GetTitle() << std::endl;
        }
    }
}