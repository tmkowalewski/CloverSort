#ifndef EVENT_HPP
#define EVENT_HPP

#include <vector>
#include <map>
#include <variant>
#include <TString.h>
#include <TTreeReaderArray.h>

class DAQModule;
class Detector;

class Event
{
public:
    // Default constructor
    Event(std::vector<DAQModule *> daq_modules, TTreeReader *ptree_reader);

    // Default destructor
    virtual ~Event();

    // Getters

    const std::vector<DAQModule *> &getDAQModules() const { return daq_modules_; }
    const Double_t getData(DAQModule *pdaq_module, const TString &filter, Int_t channel = 0);

    // Setters

    // Methods

    void addArray(DAQModule *pmodule, const TString &filter, TTreeReader *ptree_reader);
    void addValue(DAQModule *pmodule, const TString &filter, TTreeReader *ptree_reader);

private:
    TTreeReader *ptree_reader_; // Pointer to the TTreeReader for reading data
    std::vector<DAQModule *> daq_modules_;

    using ReaderVar = std::variant<
        TTreeReaderArray<Double_t>, // For double arrays
        TTreeReaderValue<Double_t>  // For doubles
        >;
    std::map<DAQModule *, std::map<TString, ReaderVar>> data_;
};

#endif // EVENT_HPP