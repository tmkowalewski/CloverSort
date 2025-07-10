// Standard C++ includes
#include <iostream>
#include <sstream>
#include <thread>

// ROOT includes
#include <TString.h>

// Project includes
#include "Utilities.hpp"

namespace Utilities
{

    std::vector<UInt_t> parseNumberString(const std::string &number_string)
    {
        std::vector<UInt_t> numbers;
        std::istringstream iss(number_string);
        std::string token;

        while (std::getline(iss, token, ','))
        {
            // Check if the token is a range
            size_t dash_pos = token.find('-');
            if (dash_pos != std::string::npos)
            {
                // Range detected
                Int_t start = std::stoi(token.substr(0, dash_pos));
                Int_t end = std::stoi(token.substr(dash_pos + 1));
                for (Int_t i = start; i <= end; ++i)
                {
                    numbers.push_back(i);
                }
            }
            else
            {
                // Single number
                numbers.push_back(std::stoi(token));
            }
        }
        return numbers;
    }

    void displayProgressBar(std::atomic<ULong64_t> &processedEntries, ULong64_t totalEntries)
    {
        const int barWidth = 50; // Width of the progress bar
        while (processedEntries < totalEntries)
        {
            double progress = static_cast<double>(processedEntries) / totalEntries;
            int pos = static_cast<int>(barWidth * progress);

            std::cout << "[";
            for (int i = 0; i < barWidth; ++i)
            {
                if (i < pos)
                    std::cout << "=";
                else if (i == pos)
                    std::cout << ">";
                else
                    std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << "% (" << processedEntries << "/" << totalEntries << ")\r";
            std::cout.flush();

            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Update every 100ms
        }
        std::cout << "[";
        for (int i = 0; i < barWidth; ++i)
            std::cout << "=";
        std::cout << "] 100% (" << totalEntries << "/" << totalEntries << ")\n";
    }
} // namespace Utilities