#include <iostream>
#include "Detector.hpp"

Detector::Detector(TString name, TString type, std::vector<Int_t> channels, DAQModule *pdaq_module)
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
        channel_list += (TString)std::to_string(channels_[i]);
        if (i != channels_.size() - 1)
            channel_list += ",";
    }
    std::cout << Form("%s (%s) [%s]", name_.Data(), type_.Data(), channel_list.Data()) << std::endl;
}