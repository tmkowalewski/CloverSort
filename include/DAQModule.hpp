#ifndef DAQMODULE_HPP
#define DAQMODULE_HPP

#define DAQ_BINS 65536    // 16-bit digitizer
#define DAQ_BINS2NS 0.098 // 0.098 ns/bin (when binned in 16-bit time spectrum)
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

    virtual const TString &getName() const { return module_name_; }
    virtual const TString &getType() const { return MODULE_TYPE_; }
    virtual const Int_t getChannelNum() const { return CHANNEL_NUM_; }
    virtual const TString &getChannelName(Int_t channel) const;
    virtual const Int_t getChannel(const TString &channel_name) const;
    virtual const std::vector<TString> &getFilters() const { return filters_; }
    virtual const std::vector<Detector *> getDetectors() const { return detectors_; }
    virtual const Detector *getDetector(const TString &detectorName) const;

    // Setters

    virtual void setName(const TString &name) { module_name_ = name; }
    virtual void setChannelName(const Int_t channel, const TString &channel_name);

    // Methods

    virtual void generateDefaultFilters();
    void addFilter(const TString &filterName);
    void removeFilter(const TString &filterName);

    virtual void addDetector(Detector *detector);
    virtual void removeDetector(Detector *detector);

    void printInfo() const;

    // Class consts
    static const std::vector<TString> VALID_MODULE_TYPES_; // Valid module types

protected:
    TString module_name_;                // Name of the module as defined in MVME
    const TString MODULE_TYPE_;          // Type of the module as defined in MVME
    const Int_t CHANNEL_NUM_;            // Number of channels in the module
    std::vector<TString> channel_names_; // Map of channels, where key is the channel number and value is the channel name
    std::vector<TString> filters_;       // List of filters associated with the module
    std::vector<Detector *> detectors_;  // List of detectors associated with this module
};

#endif // DAQMODULE_HPP