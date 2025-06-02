#include <iostream>
#include <stdexcept>
#include "DAQModule.hpp"

DAQModule::DAQModule(TString moduleName, TString moduleType, Int_t channelNum = 16)
    : moduleName_(moduleName), moduleType_(moduleType), channelNum_(channelNum), channelNames_(channelNum_)
{
    // Initialize the module with the given name and type
    generateDefaultFilters();
}

DAQModule::~DAQModule()
{
}

const TString &DAQModule::getName() const
{
    return moduleName_;
}
const TString &DAQModule::getType() const
{
    return moduleType_;
}

const TString &DAQModule::getChannelName(const Int_t channel) const
{
    return channelNames_.at(channel);
}

const Int_t DAQModule::getChannel(const TString &channelName) const
{
    auto it = std::find(channelNames_.begin(), channelNames_.end(), channelName);
    if (it != channelNames_.end())
    {
        return std::distance(channelNames_.begin(), it);
    }
    throw std::out_of_range("Channel name not found");
}

const std::vector<TString> &DAQModule::getFilters() const
{
    return filters_;
}

void DAQModule::setName(const TString moduleName)
{
    moduleName_ = moduleName;
}

void DAQModule::setType(const TString moduleType)
{
    moduleType_ = moduleType;
}

void DAQModule::setChannelName(const Int_t channel, const TString channelName)
{
    channelNames_.at(channel) = channelName;
}

void DAQModule::generateDefaultFilters()
{
    // Generate default filters based on the module type
    if (moduleType_ == "mdpp16scp")
    {
        filters_.push_back("amplitude");
        filters_.push_back("channel_time");
        filters_.push_back("module_timestamp");
        filters_.push_back("trigger_time");
    }
    else if (moduleType_ == "mdpp16qdc")
    {
        filters_.push_back("channel_time");
        filters_.push_back("integration_long");
        filters_.push_back("integration_short");
        filters_.push_back("module_timestamp");
        filters_.push_back("trigger_time");
    }
    else
    {
        std::cerr << "CloverSort [WARN]: No default filters defined for module type " << moduleType_ << ". Please define them manually using DAQModule::addFilter()" << std::endl;
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
        std::cerr << "CloverSort [WARN]: Filter " << filterName << " already exists in module " << moduleName_ << std::endl;
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
        std::cerr << "CloverSort [WARN]: Filter " << filterName << " not found in module " << moduleName_ << std::endl;
    }
}
