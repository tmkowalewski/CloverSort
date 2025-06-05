#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <vector>
#include <TString.h>

class DAQModule;

class Detector
{
public:
    Detector(TString name, TString type, std::vector<Int_t> channels);
    virtual ~Detector();

    // Getters

    const TString &getName() const;
    const TString &getType() const;
    const std::vector<Int_t> *getChannels() const;

    // Setters

    void setName(const TString name);
    void setType(const TString type);
    void setChannels(const std::vector<Int_t> channels);

    // Operators
    bool operator==(const Detector &other) const
    {
        return (name_ == other.name_) && (type_ == other.type_) && (channels_ == other.channels_) && (pdaq_module_ == other.pdaq_module_);
    }

    // Example virtual function for derived detectors

protected:
    TString name_;                           // Name of the detector
    TString type_;                           // Type of the detector
    std::vector<Int_t> channels_;            // List of channels associated with the detector
    const DAQModule *pdaq_module_ = nullptr; // Pointer to the DAQModule this detector belongs to, if applicable
};

#endif // DETECTOR_HPP