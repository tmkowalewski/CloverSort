// Standard C++ includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

// ROOT includes
#include <TString.h>

// Project includes
#include "Experiment.hpp"
#include "DAQModule.hpp"
#include "Detector.hpp"
#include "Run.hpp"
#include "Utilities.hpp"

// Constructor
Experiment::Experiment(const std::string &file_name)
    : file_name_(file_name),
      daq_modules_(), runs_()
{
    // Set the file and check if it exists
    std::ifstream config_file(file_name);
    if (!config_file.is_open())
    {
        throw std::runtime_error(Form("Could not open configuration file: %s", file_name_.c_str()));

        return;
    }

    std::cout << "CloverSort [INFO]: Defining Experiment from configuration file " << file_name_.c_str() << std::endl;

    // Read the configuration file
    std::string line;
    std::string current_section;
    std::string run_filename_pattern;

    while (std::getline(config_file, line))
    {
        // Trim whitespace
        Size_t line_start = line.find_first_not_of(" \t");
        Size_t line_end = line.find_last_not_of(" \t");

        // If the line is empty after trimming, skip it
        if (line_start == std::string::npos)
            continue;
        std::string trimmed_line = line.substr(line_start, line_end - line_start + 1);

        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
            continue;

        // Check for section headers
        if (trimmed_line == "Experiment" || trimmed_line == "DAQModules" || trimmed_line == "Detectors" || trimmed_line == "Runs")
        {
            current_section = trimmed_line;
            // std::cout << "CloverSort [INFO]: Entering section " << current_section << std::endl;
            continue;
        }
        std::istringstream iss(trimmed_line);

        // std::cout << "CloverSort [INFO]: Processing line: " << trimmed_line << std::endl;

        // Handle Experiment definition
        if (current_section == "Experiment")
        {
            // Format: OptionName    Value
            std::string option;
            iss >> option;
            std::string value;
            std::getline(iss, value);
            // Trim value
            size_t valStart = value.find_first_not_of(" \t");
            if (valStart != std::string::npos)
                value = value.substr(valStart);
            if (option == "Name")
            {
                name_ = value.c_str();
                std::cout << "CloverSort [INFO]: Setting Experiment name to " << name_ << std::endl;
            }
            std::cout << "CloverSort [INFO]: Setting Experiment name to " << name_ << std::endl;
            // Other options (e.g. FilenamePattern) can be parsed similarly.
        }
        // Handle Module definitions
        else if (current_section == "DAQModules")
        {
            // Format: module_name    module_type   filters...]
            std::string module_name, module_type;
            if (!(iss >> module_name >> module_type))
                continue;

            // Check if module_name is in the list of valid module names
            if (std::find(DAQModule::VALID_MODULE_TYPES_.begin(), DAQModule::VALID_MODULE_TYPES_.end(), module_type) == DAQModule::VALID_MODULE_TYPES_.end())
            {
                throw std::runtime_error("Unsupported module type: " + module_type);
            }
            // Create a DAQModule object and parse filters
            DAQModule *pmodule = new DAQModule(module_name.c_str(), module_type.c_str());
            std::string filter;
            while (iss >> filter)
            {
                if (filter == "default")
                {
                    pmodule->GenerateDefaultFilters();
                }
                else
                {
                    // Add the filter to the module
                    pmodule->AddFilter(filter.c_str());
                }
            }
            // Add the module to the list of DAQ modules
            daq_modules_.push_back(pmodule);
        }
        // Handle Detector definitions
        else if (current_section == "Detectors")
        {
            // Format: detector_name    detector_type module_name detector_channels
            std::string detector_name, detector_type, detector_module, detector_channels;
            if (!(iss >> detector_name >> detector_type >> detector_module >> detector_channels))
                continue;

            // Parse the channels, which are given as a range like 0-3 or comma separated like 0,1,2,3
            std::vector<UInt_t> detector_channels_parsed = Utilities::parseNumberString(detector_channels.c_str());
            // Find the DAQModule this detector belongs to
            DAQModule *pdaq_module = nullptr;
            for (DAQModule *pmodule : daq_modules_)
            {
                if ((std::string)pmodule->GetName() == detector_module)
                {
                    pdaq_module = pmodule;
                    break;
                }
            }
            // Create a Detector object and add it to the appropriate DAQModule
            if (!daq_modules_.empty())
            {
                Detector *pdetector = new Detector(detector_name.c_str(), detector_type.c_str(), detector_channels_parsed, pdaq_module);
                pdaq_module->AddDetector(pdetector);
            }
        }
        // Handle Run definitions
        else if (current_section == "Runs")
        {
            // Check for FilenamePattern line
            if (trimmed_line.find("FilenamePattern") == 0)
            {
                // Format: FilenamePattern    /absolute/path/to/file---.root
                std::string keyword, pattern;
                iss >> keyword;
                std::getline(iss, pattern);
                size_t valStart = pattern.find_first_not_of(" \t");
                if (valStart != std::string::npos)
                    pattern = pattern.substr(valStart);
                // Store the pattern for use in subsequent run lines
                run_filename_pattern = pattern;
                continue;
            }

            // Format: run_number    run_description    run_type    tree_name
            std::string run_numbers, run_description, run_type, tree_name;
            if (!(iss >> run_numbers >> run_description >> run_type >> tree_name))
                continue;

            std::vector<UInt_t> run_numbers_parsed = Utilities::parseNumberString(run_numbers.c_str());

            for (UInt_t run_number : run_numbers_parsed)
            {
                std::string file_name;
                if (!run_filename_pattern.empty() && run_filename_pattern.find("---") != std::string::npos)
                {
                    std::ostringstream oss;
                    oss << std::setw(3) << std::setfill('0') << run_number;
                    file_name = run_filename_pattern;
                    file_name.replace(file_name.find("---"), 3, oss.str());
                }
                else
                {
                    file_name = run_filename_pattern;
                }

                Run *prun = new Run(run_number, run_description.c_str(), run_type.c_str(), file_name.c_str(), tree_name.c_str());
                runs_.push_back(prun);
            }
        }
    }

    std::cout << "CloverSort [INFO]: Experiment " << name_ << " defined successfully." << std::endl;

    PrintInfo();
}

// Destructor
Experiment::~Experiment()
{
    // Clean up resources if needed
}

const DAQModule *Experiment::GetDAQModule(const std::string &module_name) const
{
    auto it = std::find_if(daq_modules_.begin(), daq_modules_.end(),
                           [&](const DAQModule *m)
                           { return m->GetName() == module_name; });
    if (it != daq_modules_.end())
        return *it;
    return nullptr;
}

const Run *Experiment::GetRun(const UInt_t run_number) const
{
    auto it = std::find_if(runs_.begin(), runs_.end(),
                           [run_number](const Run *run)
                           { return run->GetRunNumber() == run_number; });
    return (it != runs_.end()) ? *it : nullptr;
}

void Experiment::AddDAQModule(DAQModule *module)
{
    daq_modules_.push_back(module);
}

void Experiment::RemoveDAQModule(const std::string &module_name)
{
    auto it = std::remove_if(daq_modules_.begin(), daq_modules_.end(),
                             [module_name](const DAQModule *pmodule)
                             { return pmodule->GetName() == module_name; });
    if (it != daq_modules_.end())
    {
        daq_modules_.erase(it, daq_modules_.end());
    }
}

void Experiment::AddRun(Run *run)
{
    runs_.push_back(run);
}

void Experiment::RemoveRun(const UInt_t run_number)
{
    auto it = std::remove_if(runs_.begin(), runs_.end(),
                             [run_number](const Run *prun)
                             { return prun->GetRunNumber() == run_number; });
    if (it != runs_.end())
    {
        runs_.erase(it, runs_.end());
    }
}

void Experiment::PrintInfo() const
{
    std::cout << Form("Experiment %s (%s)", name_.c_str(), file_name_.c_str()) << std::endl;

    // DAQ Modules
    std::cout << "├── Modules" << std::endl;
    for (size_t i = 0; i < daq_modules_.size(); ++i)
    {
        const auto pmodule = daq_modules_[i];
        bool is_last_module = (i == daq_modules_.size() - 1);
        std::cout << "│   " << (is_last_module ? "└── " : "├── ");
        pmodule->PrintInfo();

        // Print detectors for this module, indented under the module
        const auto detectors = pmodule->GetDetectors();
        if (!detectors.empty())
        {
            for (size_t j = 0; j < detectors.size(); ++j)
            {
                bool is_last_detector = (j == detectors.size() - 1);
                // Indent to show detector is under its module
                std::cout << "│   " << (is_last_module ? "    " : "│   ");
                std::cout << (is_last_detector ? "    └── " : "    ├── ");
                detectors[j]->PrintInfo();
            }
        }
    }

    // Runs
    std::cout << "└── Runs" << std::endl;
    for (size_t i = 0; i < runs_.size(); ++i)
    {
        std::cout << "    " << (i == runs_.size() - 1 ? "└── " : "├── ");
        runs_[i]->PrintInfo();
    }
}
