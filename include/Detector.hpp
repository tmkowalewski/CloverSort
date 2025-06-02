#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <vector>
#include <TString.h>

// Forward declarations

class DAQModule;

class Detector
{
public:
    Detector(TString name, TString type = "Generic", DAQModule *daqModule = nullptr);
    virtual ~Detector();

    // Getters

    const TString &getName() const;
    const TString &getType() const;
    const std::vector<Int_t> &getChannels() const;
    const DAQModule *getDAQModule() const;

    // Setters

    void setName(const TString &name);
    void setType(const TString &type);
    void setChannels(const std::vector<Int_t> &channels);

    // Example virtual function for derived detectors

protected:
    TString name_;                // Name of the detector
    TString type_;                // Type of the detector
    std::vector<Int_t> channels_; // List of channels associated with the detector
    DAQModule *daqModule_;        // Pointer to the DAQ module associated with this detector
};

#endif // DETECTOR_HPP