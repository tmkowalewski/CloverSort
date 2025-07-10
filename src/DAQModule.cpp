// Standard C++ includes
#include <iostream>
#include <stdexcept>
#include <algorithm>

// ROOT includes
#include <TString.h>

// Project includes
#include "DAQModule.hpp"
#include "Detector.hpp"

const std::vector<std::string> DAQModule::VALID_MODULE_TYPES_ = VALID_MODULE_TYPES;

DAQModule::DAQModule(const std::string &module_name, const std::string &module_type)
    : MODULE_NAME_(module_name), MODULE_TYPE_(module_type), CHANNEL_NUM_(16), channel_names_()
{
    // Initialize the module with the given name and type
    auto it = std::find_if(VALID_MODULE_TYPES_.begin(), VALID_MODULE_TYPES_.end(), [module_type](const std::string &type)
                           { return type == module_type; });
    if (it == VALID_MODULE_TYPES_.end())
    {
        throw std::invalid_argument(Form("Invalid module type '%s' for module '%s'. Valid types are found in DAQModule.hpp, new types will have to be defined manually.", MODULE_TYPE_.c_str(), MODULE_NAME_.c_str()));
    }

    // Generate default filters based on the module type
    GenerateDefaultFilters();

    // Initialize channel names based on the number of channels
    for (UInt_t i = 0; i < CHANNEL_NUM_; ++i)
    {
        channel_names_.push_back(Form("Channel_%i", i + 1));
    }
}

DAQModule::~DAQModule()
{
}

const UInt_t DAQModule::GetChannel(const std::string &channel_name) const
{
    auto it = std::find_if(channel_names_.begin(), channel_names_.end(), [channel_name](const std::string &name)
                           { return name == channel_name; });
    if (it != channel_names_.end())
    {
        return std::distance(channel_names_.begin(), it);
    }
    throw std::out_of_range("Channel name not found");
}

const Detector *DAQModule::GetDetector(const std::string &detector_name) const
{
    auto it = std::find_if(detectors_.begin(), detectors_.end(), [detector_name](const Detector *pdetector)
                           { return pdetector->GetName() == detector_name; });
    if (it != detectors_.end())
    {
        return *it;
    }
    throw std::out_of_range(Form("Detector '%s' not found in module '%s'", detector_name.c_str(), MODULE_NAME_.c_str()));
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

void DAQModule::AddFilter(const std::string &filter_name)
{
    // Add a filter to the module
    if (std::find_if(filters_.begin(), filters_.end(), [filter_name](const std::string &name)
                     { return name == filter_name; }) == filters_.end())
    {
        filters_.push_back(filter_name);
    }
    else
    {
        std::cerr << "CloverSort [WARN]: Filter " << filter_name << " already exists in module " << MODULE_NAME_ << std::endl;
    }
}

void DAQModule::RemoveFilter(const std::string &filter_name)
{
    // Remove a filter from the module
    auto it = std::find_if(filters_.begin(), filters_.end(), [filter_name](const std::string &name)
                           { return name == filter_name; });
    if (it != filters_.end())
    {
        filters_.erase(it);
    }
    else
    {
        std::cerr << "CloverSort [WARN]: Filter " << filter_name << " not found in module " << MODULE_NAME_ << std::endl;
    }
}

void DAQModule::AddDetector(Detector *pdetector)
{
    // Add a detector to the module
    detectors_.push_back(pdetector);
    for (Int_t channel : pdetector->GetChannels())
    {
        channel_names_.push_back(Form("%sE%i", pdetector->GetName().c_str(), channel % 4 + 1)); // Assuming channels are 0-indexed and we want to name them as E1, E2, etc.
    }
}

void DAQModule::RemoveDetector(Detector *pdetector)
{
    // Remove a detector from the module
    auto it = std::find_if(detectors_.begin(), detectors_.end(), [pdetector](const Detector *d)
                           { return d == pdetector; });
    if (it != detectors_.end())
    {
        detectors_.erase(it);
        channel_names_.at(std::distance(detectors_.begin(), it)) = ""; // Clear the channel name associated with the detector
    }
    else
    {
        std::cerr << "CloverSort [WARN]: Detector " << pdetector->GetName() << " not found in module " << MODULE_NAME_ << std::endl;
    }
}

void DAQModule::PrintInfo() const
{
    // Print module information
    std::cout << Form("%s (%s) [%i]", MODULE_NAME_.c_str(), MODULE_TYPE_.c_str(), CHANNEL_NUM_) << std::endl;
}