#ifndef DAQMODULE_HPP
#define DAQMODULE_HPP

#define DAQ_BINS 65536    // 16-bit digitizer
#define DAQ_BINS2NS 0.098 // 0.098 ns/bin (when binned in 16-bit time spectrum)
#define VALID_MODULE_TYPES {"mdpp16scp", "mdpp16qdc"}

// Standard C++ includes
#include <vector>
#include <string>

// ROOT includes
#include <Rtypes.h>

// Project includes

// Forward declarations
class Detector;

class DAQModule
{
public:
    // Constructors

    DAQModule(const std::string &name, const std::string &type);

    // Default destructor method
    virtual ~DAQModule();

    // Getters

    virtual const std::string &GetName() const { return MODULE_NAME_; }
    virtual const std::string &GetType() const { return MODULE_TYPE_; }
    virtual const UInt_t GetChannelNum() const { return CHANNEL_NUM_; }
    virtual const std::string &GetChannelName(UInt_t channel) const { return channel_names_.at(channel); }
    virtual const UInt_t GetChannel(const std::string &channel_name) const;
    virtual const std::vector<std::string> &GetFilters() const { return filters_; }
    virtual const std::vector<Detector *> GetDetectors() const { return detectors_; }
    virtual const Detector *GetDetector(const std::string &detectorName) const;

    // Setters

    virtual void SetName(const std::string &name) { MODULE_NAME_ = name; }
    virtual void SetChannelName(const UInt_t channel, const std::string &channel_name) { channel_names_.at(channel) = channel_name; }

    // Methods

    virtual void GenerateDefaultFilters();
    void AddFilter(const std::string &filterName);
    void RemoveFilter(const std::string &filterName);

    virtual void AddDetector(Detector *detector);
    virtual void RemoveDetector(Detector *detector);

    void PrintInfo() const;

    // Class consts
    static const std::vector<std::string> VALID_MODULE_TYPES_; // Valid module types

protected:
    std::string MODULE_NAME_;                // Name of the module as defined in MVME
    std::string MODULE_TYPE_;                // Type of the module as defined in MVME
    UInt_t CHANNEL_NUM_;                     // Number of channels in the module
    std::vector<std::string> channel_names_; // Map of channels, where key is the channel name and value is the channel n
    std::vector<std::string> filters_;       // List of filters associated with the module
    std::vector<Detector *> detectors_;      // List of detectors associated with this module
};

#endif // DAQMODULE_HPP