#include "Detector.hpp"

Detector::Detector(TString name, TString type, std::vector<Int_t> channels)
    : name_(name), type_(type), channels_(channels)
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

void Detector::setName(const TString name)
{
    name_ = name;
}

void Detector::setType(const TString type)
{
    type_ = type;
}

void Detector::setChannels(const std::vector<Int_t> channels)
{
    channels_ = channels;
}
