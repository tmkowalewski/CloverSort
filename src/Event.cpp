// Standard C++ includes

// ROOT includes
#include <TTreeReader.h>
#include <TObjArray.h>

// Project includes
#include "Experiment.hpp"
#include "Event.hpp"
#include "DAQModule.hpp"
#include "Detector.hpp"

Event::Event(Experiment *pexperiment, TTreeReader *ptree_reader)
    : pexperiment_(pexperiment), ptree_reader_(ptree_reader)
{
    for (const auto &pdaq_module : pexperiment->GetDAQModules())
    {
        auto module_name = pdaq_module->GetName();

        // Create module-specific histograms (e.g., module_timestamp)
        for (const auto &filter : pdaq_module->GetFilters())
        {

            if (filter == "module_timestamp")
            {
                AddDataValue(module_name, filter);
            }
            else
            {
                AddDataArray(module_name, filter);
            }
        }
    }
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
        return *value.Get();
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

void Event::PrintInfo() const
{
    std::cout << "Event Data:" << std::endl;
    for (const auto &owner_pair : data_)
    {
        std::cout << "Owner: " << owner_pair.first << std::endl;
        for (const auto &filter_pair : owner_pair.second)
        {
            std::cout << "  Filter: " << filter_pair.first << std::endl;
            if (std::holds_alternative<TTreeReaderArray<Double_t>>(filter_pair.second))
            {
                const auto &array = std::get<TTreeReaderArray<Double_t>>(filter_pair.second);
                std::cout << "    Type: TTreeReaderArray, Size: " << array.GetSize() << std::endl;
            }
            else if (std::holds_alternative<TTreeReaderValue<Double_t>>(filter_pair.second))
            {
                auto &value = std::get<TTreeReaderValue<Double_t>>(filter_pair.second);
                std::cout << "    Type: TTreeReaderValue, Value: " << const_cast<TTreeReaderValue<Double_t> &>(value).Get() << std::endl;
            }
        }
    }
}