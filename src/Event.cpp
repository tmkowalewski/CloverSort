#include "Event.hpp"
#include "DAQModule.hpp"

Event::Event(std::vector<DAQModule *> daq_modules, TTreeReader *ptree_reader)
    : ptree_reader_(ptree_reader),
      daq_modules_(daq_modules)
{
    for (DAQModule *pmodule : daq_modules_)
    {
        for (const TString &filter : *pmodule->getFilters())
        {
            try
            {
                addArray(pmodule, filter, ptree_reader);
            }
            catch (const std::bad_variant_access &)
            {
                // Handle the case where the variant type is not TTreeReaderArray<Double_t>
                // This can happen if the filter is not an array type
                addValue(pmodule, filter, ptree_reader);
            }
        }
    }
}

Event::~Event()
{
}

const Double_t Event::getData(DAQModule *pdaq_module, const TString &filter, Int_t channel)
{
    auto it = data_.find(pdaq_module);
    if (it == data_.end())
    {
        throw std::runtime_error("DAQModule not found in data map");
    }
    auto jt = it->second.find(filter);
    if (jt == it->second.end())
    {
        throw std::runtime_error("Filter not found in data map for the specified DAQModule");
    }
    return std::visit([&](auto &reader)
                      {
        using T = std::decay_t<decltype(reader)>;
        if constexpr (std::is_same_v<T, TTreeReaderArray<Double_t>>) {
            return reader.At(channel);
        } else if constexpr (std::is_same_v<T, TTreeReaderValue<Double_t>>) {
            return *reader;
        } else {
            throw std::runtime_error("Unsupported ReaderVar type");
        } }, jt->second);
}

void Event::addArray(DAQModule *pmodule, const TString &filter, TTreeReader *ptree_reader)
{
    // Names in the TTree are "module_name.filter_name", but this is redundant
    // since the module name is already part of the DAQModule object.
    // So we can just use filter directly as the name for the TTreeReaderArray,
    // but we still need to grab module_name.filter_name from the TTree.
    data_[pmodule].insert_or_assign(filter, TTreeReaderArray<Double_t>(*ptree_reader, pmodule->getName() + "." + filter));
}

void Event::addValue(DAQModule *pmodule, const TString &filter, TTreeReader *ptree_reader)
{
    // Names in the TTree are "module_name.filter_name", but this is redundant
    // since the module name is already part of the DAQModule object.
    // So we can just use filter directly as the name for the TTreeReaderArray,
    // but we still need to grab module_name.filter_name from the TTree.
    data_[pmodule].insert_or_assign(filter, TTreeReaderValue<Double_t>(*ptree_reader, pmodule->getName() + "." + filter));
}