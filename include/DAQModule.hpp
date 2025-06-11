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

    virtual const TString &GetName() const { return module_name_; }
    virtual const TString &GetType() const { return MODULE_TYPE_; }
    virtual const Int_t GetChannelNum() const { return CHANNEL_NUM_; }
    virtual const TString &GetChannelName(Int_t channel) const;
    virtual const Int_t GetChannel(const TString &channel_name) const;
    virtual const std::vector<TString> &GetFilters() const { return filters_; }
    virtual const std::vector<Detector *> GetDetectors() const { return detectors_; }
    virtual const Detector *GetDetector(const TString &detectorName) const;

    // Setters

    virtual void SetName(const TString &name) { module_name_ = name; }
    virtual void SetChannelName(const Int_t channel, const TString &channel_name);

    // Methods

    virtual void GenerateDefaultFilters();
    void AddFilter(const TString &filterName);
    void RemoveFilter(const TString &filterName);

    virtual void AddDetector(Detector *detector);
    virtual void RemoveDetector(Detector *detector);

    void PrintInfo() const;

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