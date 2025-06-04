#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <vector>
#include <TString.h>

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

    // Example virtual function for derived detectors

protected:
    TString name_;                // Name of the detector
    TString type_;                // Type of the detector
    std::vector<Int_t> channels_; // List of channels associated with the detector
};

#endif // DETECTOR_HPP