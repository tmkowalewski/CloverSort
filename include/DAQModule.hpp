#ifndef DAQMODULE_HPP
#define DAQMODULE_HPP

#include <vector>
#include <TString.h>

class DAQModule
{
public:
    // Constructors

    DAQModule(TString name, TString type, Int_t channelNum);

    // Default destructor method
    virtual ~DAQModule();

    // Getters

    virtual const TString &getName() const;
    virtual const TString &getType() const;

    virtual const TString &getChannelName(const Int_t channel) const;
    virtual const Int_t getChannel(const TString &channelName) const;
    virtual const std::vector<TString> &getFilters() const;

    // Setters

    virtual void setName(const TString name);
    virtual void setType(const TString type);
    virtual void setChannelName(const Int_t channel, const TString channelName);

    // Methods

    virtual void generateDefaultFilters();
    void addFilter(const TString filterName);
    void removeFilter(const TString filterName);

protected:
    TString moduleName_; // Name of the module as defined in MVME
    TString moduleType_; // Type of the module as defined in MVME

    std::vector<TString> filters_; // List of filters associated with the module

    const Int_t channelNum_;            // Number of channels in the module
    std::vector<TString> channelNames_; // Map of channels, where key is the channel number and value is the channel name
};

#endif // DAQMODULE_HPP