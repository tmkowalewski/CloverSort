// Standard C++ includes
#include <iostream>
#include <string>

// ROOT includes
#include <TString.h>

// Project includes
#include "Detector.hpp"

Detector::Detector(const std::string &name, const std::string &type, std::vector<UInt_t> channels, DAQModule *pdaq_module)
    : name_(name), type_(type), channels_(channels), pdaq_module_(pdaq_module)
{
}

Detector::~Detector()
{
}

void Detector::PrintInfo() const
{
    // Print the detector information
    TString channel_list;
    for (Size_t i = 0; i < channels_.size(); ++i)
    {
        channel_list += std::to_string(channels_[i]);
        if (i != channels_.size() - 1)
            channel_list += ',';
    }
    std::cout << Form("%s (%s) [%s]", name_.c_str(), type_.c_str(), channel_list.Data()) << std::endl;
}