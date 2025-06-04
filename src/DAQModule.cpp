#include <iostream>
#include <stdexcept>
#include "DAQModule.hpp"
#include "Detector.hpp"

const std::vector<TString> DAQModule::VALID_MODULE_TYPES_ = VALID_MODULE_TYPES;

DAQModule::DAQModule(TString module_name, TString module_type)
    : module_name_(module_name), MODULE_TYPE_(module_type), CHANNEL_NUM_(), channel_names_(std::find(VALID_MODULE_TYPES_.begin(), VALID_MODULE_TYPES_.end(), module_type) != VALID_MODULE_TYPES_.end() ? 16 : 0)
{
}

DAQModule::~DAQModule()
{
}

const TString &DAQModule::getName() const
{
    return module_name_;
}

const TString &DAQModule::getType() const
{
    return MODULE_TYPE_;
}

const TString &DAQModule::getChannelName(const Int_t channel) const
{
    return channel_names_.at(channel);
}

const Int_t DAQModule::getChannel(const TString &channelName) const
{
    auto it = std::find(channel_names_.begin(), channel_names_.end(), channelName);
    if (it != channel_names_.end())
    {
        return std::distance(channel_names_.begin(), it);
    }
    throw std::out_of_range("Channel name not found");
}

const std::vector<TString> *DAQModule::getFilters() const
{
    return &filters_;
}

void DAQModule::setName(const TString module_name)
{
    module_name_ = module_name;
}

void DAQModule::setChannelName(const Int_t channel, const TString channelName)
{
    channel_names_.at(channel) = channelName;
}

void DAQModule::generateDefaultFilters()
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
        std::cerr << "CloverSort [WARN]: No default filters defined for module type " << MODULE_TYPE_ << ". Please define them manually using DAQModule::addFilter()" << std::endl;
    }
}

void DAQModule::addFilter(const TString filterName)
{
    // Add a filter to the module
    if (std::find(filters_.begin(), filters_.end(), filterName) == filters_.end())
    {
        filters_.push_back(filterName);
    }
    else
    {
        std::cerr << "CloverSort [WARN]: Filter " << filterName << " already exists in module " << module_name_ << std::endl;
    }
}

void DAQModule::removeFilter(const TString filterName)
{
    // Remove a filter from the module
    auto it = std::find(filters_.begin(), filters_.end(), filterName);
    if (it != filters_.end())
    {
        filters_.erase(it);
    }
    else
    {
        std::cerr << "CloverSort [WARN]: Filter " << filterName << " not found in module " << module_name_ << std::endl;
    }
}

void DAQModule::addDetector(const Detector &detector)
{
    // Add a detector to the module
    detectors_.push_back(detector);
}

void DAQModule::removeDetector(const Detector &detector)
{
    // Remove a detector from the module
    auto it = std::find(detectors_.begin(), detectors_.end(), detector);
    if (it != detectors_.end())
    {
        detectors_.erase(it);
    }
    else
    {
        std::cerr << "CloverSort [WARN]: Detector " << detector.getName() << " not found in module " << module_name_ << std::endl;
    }
}

const Detector *DAQModule::getDetector(const TString &detectorName) const
{
    // Get a detector by name
    for (const auto &detector : detectors_)
    {
        if (detector.getName() == detectorName)
        {
            return &detector;
        }
    }
    return nullptr; // Return nullptr if the detector is not found
}

void DAQModule::printInfo() const
{
    // Print module information
    std::cout << "Module Name: " << module_name_ << std::endl;
    std::cout << "Module Type: " << MODULE_TYPE_ << std::endl;
    std::cout << "Number of Channels: " << CHANNEL_NUM_ << std::endl;
    std::cout << "Channel Names: ";
    for (const auto &name : channel_names_)
    {
        std::cout << name << " ";
    }
    std::cout << std::endl;

    std::cout << "Filters: ";
    for (const auto &filter : filters_)
    {
        std::cout << filter << " ";
    }
    std::cout << std::endl;

    std::cout << "Detectors: ";
    for (const auto &detector : detectors_)
    {
        std::cout << detector.getName() << " (" << detector.getType() << ") ";
    }
    std::cout << std::endl;
}