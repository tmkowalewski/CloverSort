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

    void displayProgressBar(std::atomic<ULong64_t> &processed_entries, ULong64_t total_entries, std::chrono::_V2::system_clock::time_point start_time)
    {

        const int bar_width = 50; // Width of the progress bar
        while (processed_entries < total_entries)
        {
            // Calculate elapsed time
            auto elapsed_time = (std::chrono::high_resolution_clock::now() - start_time) / 1e9; // Convert to seconds

            double progress = static_cast<double>(processed_entries) / total_entries;
            int pos = static_cast<int>(bar_width * progress);

            std::cout << "[";
            for (int i = 0; i < bar_width; ++i)
            {
                if (i < pos)
                    std::cout << "=";
                else if (i == pos)
                    std::cout << ">";
                else
                    std::cout << " ";
            }
            double estimated_total = elapsed_time.count() / progress;
            double remaining = estimated_total - elapsed_time.count();
            int etc_h = static_cast<int>(remaining) / 3600;
            int etc_m = (static_cast<int>(remaining) % 3600) / 60;
            int etc_s = static_cast<int>(remaining) % 60;
            std::cout << Form("] %i%% (%llu/%llu) ETC %02d:%02d:%02d",
                              static_cast<int>(progress * 100.0),
                              static_cast<unsigned long long>(processed_entries),
                              static_cast<unsigned long long>(total_entries),
                              etc_h, etc_m, etc_s);
            std::cout.flush();
            std::cout << "\r" << std::string(100, ' ') << "\r"; // Clear the line

            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Update every 100ms
        }
        std::cout << "[";
        for (int i = 0; i < bar_width; ++i)
            std::cout << "=";
        std::cout << "] 100% (" << total_entries << "/" << total_entries << ")\n";
    }

} // namespace Utilities