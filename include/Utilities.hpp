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

    void displayProgressBar(std::atomic<ULong64_t> &processed_entries, ULong64_t total_entries, std::chrono::_V2::system_clock::time_point start_time);

    constexpr unsigned int str2int(const char *str, int h = 0)
    {
        return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
    }

}

#endif // UTILITIES_HPP