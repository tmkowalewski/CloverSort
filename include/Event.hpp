#ifndef EVENT_HPP
#define EVENT_HPP

// Standard C++ includes
#include <vector>
#include <unordered_map>
#include <variant>

// ROOT includes
#include <TTreeReaderArray.h>

// Project Includes

// Forward declarations
class Experiment;
class DAQModule;
class Detector;

class Event
{
public:
    using ReaderVar = std::variant<
        TTreeReaderArray<Double_t>, // For double arrays
        TTreeReaderValue<Double_t>  // For doubles
        >;
    using DataMap = std::unordered_map<std::string, std::unordered_map<std::string, ReaderVar>>;

    // Default constructor
    Event(Experiment *experiment, TTreeReader *ptree_reader);

    // Default destructor
    virtual ~Event();

    // Getters

    Double_t GetData(std::string owner, std::string filter, UInt_t index);

    // Setters

    // Methods

    void AddDataArray(std::string owner, std::string filter);
    void AddDataValue(std::string owner, std::string filter);

private:
    Experiment *pexperiment_;   // Pointer to the Experiment object
    TTreeReader *ptree_reader_; // Pointer to the TTreeReader for reading data
    DataMap data_;
};

#endif // EVENT_HPP