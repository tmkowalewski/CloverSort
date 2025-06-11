#include <iostream>
#include <stdexcept>
#include "DAQModule.hpp"
#include "Detector.hpp"

const std::vector<TString> DAQModule::VALID_MODULE_TYPES_ = VALID_MODULE_TYPES;

DAQModule::DAQModule(TString module_name, TString module_type)
    : module_name_(module_name), MODULE_TYPE_(*std::find(VALID_MODULE_TYPES_.begin(), VALID_MODULE_TYPES_.end(), module_type)), CHANNEL_NUM_(std::find(VALID_MODULE_TYPES_.begin(), VALID_MODULE_TYPES_.end(), module_type) != VALID_MODULE_TYPES_.end() ? 16 : 0), channel_names_()
{
}

DAQModule::~DAQModule()
{
}

const TString &DAQModule::GetChannelName(Int_t channel) const
{
    return channel_names_.at(channel);
}

const Int_t DAQModule::GetChannel(const TString &channel_name) const
{
    auto it = std::find(channel_names_.begin(), channel_names_.end(), channel_name);
    if (it != channel_names_.end())
    {
        return std::distance(channel_names_.begin(), it);
    }
    throw std::out_of_range("Channel name not found");
}

const Detector *DAQModule::GetDetector(const TString &detector_name) const
{
    // Get a detector by name
    for (const Detector *pdetector : detectors_)
    {
        if (pdetector->GetName() == detector_name)
        {
            return pdetector;
        }
    }
    return nullptr; // Return nullptr if the detector is not found
}

void DAQModule::SetChannelName(const Int_t channel, const TString &channel_name)
{
    channel_names_.at(channel) = channel_name;
}

void DAQModule::GenerateDefaultFilters()
{
    // Generate default filters based on the module type
    if (MODULE_TYPE_ == "mdpp16scp")
    {
        filters_.push_back("amplitude");
        filters_.push_back("channel_time");
        filters_.push_back("module_timestamp");
        filters_.push_back("trigger_time");
    }
    else if (MODULE_TYPE_ == "mdpp16qdc")
    {
        filters_.push_back("channel_time");
        filters_.push_back("integration_long");
        filters_.push_back("integration_short");
        filters_.push_back("module_timestamp");
        filters_.push_back("trigger_time");
    }
    else
    {
        std::cerr << "CloverSort [WARN]: No default filters defined for module type " << MODULE_TYPE_ << ". Please define them manually using DAQModule::AddFilter()" << std::endl;
    }
}

void DAQModule::AddFilter(const TString &filter_name)
{
    // Add a filter to the module
    if (std::find(filters_.begin(), filters_.end(), filter_name) == filters_.end())
    {
        filters_.push_back(filter_name);
    }
    else
    {
        std::cerr << "CloverSort [WARN]: Filter " << filter_name << " already exists in module " << module_name_ << std::endl;
    }
}

void DAQModule::RemoveFilter(const TString &filter_name)
{
    // Remove a filter from the module
    auto it = std::find(filters_.begin(), filters_.end(), filter_name);
    if (it != filters_.end())
    {
        filters_.erase(it);
    }
    else
    {
        std::cerr << "CloverSort [WARN]: Filter " << filter_name << " not found in module " << module_name_ << std::endl;
    }
}

void DAQModule::AddDetector(Detector *pdetector)
{
    // Add a detector to the module
    detectors_.push_back(pdetector);
}

void DAQModule::RemoveDetector(Detector *pdetector)
{
    // Remove a detector from the module
    auto it = std::find(detectors_.begin(), detectors_.end(), pdetector);
    if (it != detectors_.end())
    {
        detectors_.erase(it);
    }
    else
    {
        std::cerr << "CloverSort [WARN]: Detector " << pdetector->GetName() << " not found in module " << module_name_ << std::endl;
    }
}

void DAQModule::PrintInfo() const
{
    // Print module information
    std::cout << Form("%s (%s) [%i]", module_name_.Data(), MODULE_TYPE_.Data(), CHANNEL_NUM_) << std::endl;
}