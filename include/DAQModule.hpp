#ifndef DAQMODULE_HPP
#define DAQMODULE_HPP

#define VALID_MODULE_TYPES {"mdpp16scp", "mdpp16qdc"}

#include <vector>
#include <TString.h>

// Forward declarations
class Detector;

class DAQModule
{
public:
    // Constructors

    DAQModule(TString name, TString type);

    // Default destructor method
    virtual ~DAQModule();

    // Getters

    virtual const TString &getName() const;
    virtual const TString &getType() const;

    virtual const TString &getChannelName(const Int_t channel) const;
    virtual const Int_t getChannel(const TString &channelName) const;
    virtual const std::vector<TString> *getFilters() const;
    virtual const std::vector<Detector> *getDetectors() const;

    // Setters

    virtual void setName(const TString name);
    virtual void setChannelName(const Int_t channel, const TString channelName);

    // Methods

    virtual void generateDefaultFilters();
    void addFilter(const TString filterName);
    void removeFilter(const TString filterName);

    virtual void addDetector(const Detector &detector);
    virtual void removeDetector(const Detector &detector);
    virtual const Detector *getDetector(const TString &detectorName) const;

    void printInfo() const;

    // Class consts
    static const std::vector<TString> VALID_MODULE_TYPES_; // Valid module types

    // Instance consts
    const Int_t CHANNEL_NUM_;   // Number of channels in the module
    const TString MODULE_TYPE_; // Type of the module as defined in MVME

protected:
    TString module_name_;                // Name of the module as defined in MVME
    std::vector<TString> filters_;       // List of filters associated with the module
    std::vector<TString> channel_names_; // Map of channels, where key is the channel number and value is the channel name
    std::vector<Detector> detectors_;    // List of detectors associated with this module
};

#endif // DAQMODULE_HPP