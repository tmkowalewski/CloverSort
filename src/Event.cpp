#include "Event.hpp"
#include "DAQModule.hpp"
#include "Detector.hpp"

Event::Event(Experiment *pexperiment, TTreeReader *ptree_reader)
    : pexperiment_(pexperiment), ptree_reader_(ptree_reader)
{
}

Event::~Event()
{
}

Double_t Event::GetData(std::string owner, std::string filter, UInt_t index)
{
    // Use .find() to safely search for the owner map
    auto owner_it = data_.find(owner);
    if (owner_it == data_.end())
    {
        throw std::out_of_range("Owner '" + owner + "' not found in Event data map.");
    }

    // Use .find() to safely search for the filter within the owner's map
    auto filter_it = owner_it->second.find(filter);
    if (filter_it == owner_it->second.end())
    {
        throw std::out_of_range("Filter '" + filter + "' not found for owner '" + owner + "'.");
    }

    // Now it is safe to get a reference to the variant
    ReaderVar &reader_var = filter_it->second;
    if (std::holds_alternative<TTreeReaderArray<Double_t>>(reader_var))
    {
        TTreeReaderArray<Double_t> &array = std::get<TTreeReaderArray<Double_t>>(reader_var);
        if (index < array.GetSize())
        {
            return array[index];
        }
        else
        {
            throw std::out_of_range("Index out of range for TTreeReaderArray.");
        }
    }
    else if (std::holds_alternative<TTreeReaderValue<Double_t>>(reader_var))
    {
        if (index != 0)
        {
            throw std::out_of_range("Index must be 0 for TTreeReaderValue.");
        }
        TTreeReaderValue<Double_t> &value = std::get<TTreeReaderValue<Double_t>>(reader_var);
        return *value;
    }
    else
    {
        // This case should ideally not be reached if the map is populated correctly
        throw std::invalid_argument("Variant holds an unexpected type.");
    }
}

void Event::AddDataArray(std::string owner, std::string filter)
{
    // Use emplace on the outer map to get an iterator to the inner map.
    auto inner_map_it = data_.emplace(owner, std::unordered_map<std::string, ReaderVar>()).first;

    // Now, emplace the TTreeReaderArray into the inner map using piecewise construction.
    TString branch_name = Form("%s.%s", owner.c_str(), filter.c_str());
    inner_map_it->second.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(filter),
                                 std::forward_as_tuple(std::in_place_type<TTreeReaderArray<Double_t>>, *ptree_reader_, branch_name.Data()));
}

void Event::AddDataValue(std::string owner, std::string filter)
{
    // Use emplace on the outer map to get an iterator to the inner map.
    auto inner_map_it = data_.emplace(owner, std::unordered_map<std::string, ReaderVar>()).first;

    // Now, emplace the TTreeReaderValue into the inner map using piecewise construction.
    TString branch_name = Form("%s.%s", owner.c_str(), filter.c_str());
    inner_map_it->second.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(filter),
                                 std::forward_as_tuple(std::in_place_type<TTreeReaderValue<Double_t>>, *ptree_reader_, branch_name.Data()));
}