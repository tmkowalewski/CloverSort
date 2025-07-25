#ifndef UTILITIES_HPP
#define UTILITIES_HPP

// Standard C++ includes
#include <atomic>

// ROOT includes
#include <Rtypes.h>

// Project includes

namespace Utilities
{

    std::vector<UInt_t> parseNumberString(const std::string &number_string);

    void displayProgressBar(std::atomic<ULong64_t> &processedEntries, ULong64_t totalEntries);

}

#endif // UTILITIES_HPP