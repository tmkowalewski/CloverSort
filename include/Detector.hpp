#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <vector>
#include <TString.h>

class DAQModule;

class Detector
{
public:
    Detector(TString name, TString type, std::vector<Int_t> channels, DAQModule *pdaq_module = nullptr); // Constructor with optional DAQModule pointer
    virtual ~Detector();

    // Getters

    const TString &GetName() const { return name_; }
    const TString &GetType() const { return type_; }
    const std::vector<Int_t> &GetChannels() const { return channels_; }
    const DAQModule *GetDAQModule() const { return pdaq_module_; }

    // Setters

    void SetName(const TString &name) { name_ = name; }
    void setType(const TString &type) { type_ = type; }
    void setChannels(const std::vector<Int_t> &channels) { channels_ = channels; }
    void setDAQModule(const DAQModule *pdaq_module) { pdaq_module_ = pdaq_module; }

    // Methods
    void PrintInfo() const;

protected:
    TString name_;                           // Name of the detector
    TString type_;                           // Type of the detector
    std::vector<Int_t> channels_;            // List of channels associated with the detector
    const DAQModule *pdaq_module_ = nullptr; // Pointer to the DAQModule this detector belongs to, if applicable
};

#endif // DETECTOR_HPP