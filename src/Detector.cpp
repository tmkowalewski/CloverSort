#include <iostream>
#include "Detector.hpp"

Detector::Detector(TString name, TString type, std::vector<Int_t> channels, DAQModule *pdaq_module)
    : name_(name), type_(type), channels_(channels), pdaq_module_(pdaq_module)
{
}

Detector::~Detector()
{
}

const TString &Detector::getName() const
{
    return name_;
}

const TString &Detector::getType() const
{
    return type_;
}

const std::vector<Int_t> *Detector::getChannels() const
{
    return &channels_;
}

void Detector::setName(const TString &name)
{
    name_ = name;
}

void Detector::setType(const TString &type)
{
    type_ = type;
}

void Detector::setChannels(const std::vector<Int_t> &channels)
{
    channels_ = channels;
}

const DAQModule *Detector::getDAQModule() const
{
    return pdaq_module_;
}

void Detector::setDAQModule(const DAQModule *pdaq_module)
{
    pdaq_module_ = pdaq_module;
}

void Detector::printInfo() const
{
    // Print the detector information
    TString channel_list;
    for (Size_t i = 0; i < channels_.size(); ++i)
    {
        channel_list += (TString)std::to_string(channels_[i]);
        if (i != channels_.size() - 1)
            channel_list += ",";
    }
    std::cout << Form("%s (%s) [%s]", name_.Data(), type_.Data(), channel_list.Data()) << std::endl;
}