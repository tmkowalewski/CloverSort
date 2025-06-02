#include "MDPP16SCP.hpp"

// Constructor
MDPP16SCP::MDPP16SCP(TString name, std::map<TString, TString> &channelmap)
    : DAQModule(name, moduleType) // Call the base class constructor with name and type
{
}

// Destructor
MDPP16SCP::~MDPP16SCP()
{
    // Cleanup code here
}