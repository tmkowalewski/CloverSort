#include "HistogramManager.hpp"

HistogramManager::HistogramManager()
{
    // Constructor implementation
}

HistogramManager::~HistogramManager()
{
    // Destructor implementation
    for (auto &pair : histograms_)
    {
        for (auto *histogram : pair.second)
        {
            delete histogram; // Clean up each histogram
        }
    }
}