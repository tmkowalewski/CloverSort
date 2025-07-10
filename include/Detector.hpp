#ifndef DETECTOR_HPP
#define DETECTOR_HPP

// Standard C++ includes
#include <vector>
#include <string>

// ROOT includes
#include <Rtypes.h>

// Project includes

// Forward declarations
class DAQModule;

class Detector
{
public:
    Detector(const std::string &name, const std::string &type, std::vector<UInt_t> channels, DAQModule *pdaq_module = nullptr); // Constructor with optional DAQModule pointer
    virtual ~Detector();

    // Getters

    const std::string &GetName() const { return name_; }
    const std::string &GetType() const { return type_; }
    const std::vector<UInt_t> &GetChannels() const { return channels_; }
    const DAQModule *GetDAQModule() const { return pdaq_module_; }

    // Setters

    void SetName(const std::string &name) { name_ = name; }
    void SetType(const std::string &type) { type_ = type; }
    void SetChannels(const std::vector<UInt_t> &channels) { channels_ = channels; }
    void SetDAQModule(const DAQModule *pdaq_module) { pdaq_module_ = pdaq_module; }

    // Methods
    void PrintInfo() const;

protected:
    std::string name_;                       // Name of the detector
    std::string type_;                       // Type of the detector
    std::vector<UInt_t> channels_;           // List of channels associated with the detector
    const DAQModule *pdaq_module_ = nullptr; // Pointer to the DAQModule this detector belongs to, if applicable
};

#endif // DETECTOR_HPP