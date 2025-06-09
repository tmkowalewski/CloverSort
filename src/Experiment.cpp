#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "Experiment.hpp"
#include "DAQModule.hpp"
#include "Detector.hpp"
#include "Run.hpp"

// Helpers

std::vector<Int_t> parseNumberString(const TString &number_string)
{
    std::vector<Int_t> numbers;
    std::istringstream iss(number_string.Data());
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

// Constructor
Experiment::Experiment(const TString file_name)
    : file_name_(file_name),
      daq_modules_(), runs_()
{
    // Set the file and check if it exists
    std::ifstream config_file(file_name.Data());
    if (!config_file.is_open())
    {
        throw std::runtime_error(
            std::string("Could not open configuration file: ") + file_name.Data());

        return;
    }

    std::cout << "CloverSort [INFO]: Defining Experiment from configuration file " << file_name.Data() << std::endl;

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
        if (trimmed_line == "ExperimentOptions" || trimmed_line == "DAQModules" || trimmed_line == "Detectors" || trimmed_line == "Runs")
        {
            current_section = trimmed_line;
            std::cout << "CloverSort [INFO]: Entering section " << current_section << std::endl;
            continue;
        }
        std::istringstream iss(trimmed_line);

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
            }
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
            DAQModule *pmodule = new DAQModule(module_name, module_type);
            std::string filter;
            while (iss >> filter)
            {
                if (filter == "default")
                {
                    pmodule->generateDefaultFilters();
                }
                else
                {
                    // Add the filter to the module
                    pmodule->addFilter(filter);
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
            std::vector<Int_t> detector_channels_parsed = parseNumberString(detector_channels);
            // Find the DAQModule this detector belongs to
            DAQModule *pdaq_module = nullptr;
            for (DAQModule *pmodule : daq_modules_)
            {
                if (pmodule->getName() == detector_module)
                {
                    pdaq_module = pmodule;
                    break;
                }
            }
            // Create a Detector object and add it to the appropriate DAQModule
            if (!daq_modules_.empty())
            {
                Detector *pdetector = new Detector(detector_name, detector_type, detector_channels_parsed, pdaq_module);
                pdaq_module->addDetector(pdetector);
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

            std::vector<Int_t> run_numbers_parsed = parseNumberString(run_numbers);

            for (Int_t run_number : run_numbers_parsed)
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

                Run *prun = new Run(run_number, run_description, run_type, file_name, tree_name);
                runs_.push_back(prun);
            }
        }
    }

    std::cout << "CloverSort [INFO]: Experiment " << name_.Data() << " defined successfully." << std::endl;

    printInfo();
}

// Destructor
Experiment::~Experiment()
{
    // Clean up resources if needed
}

const DAQModule *Experiment::getDAQModule(const TString &module_name) const
{
    auto it = std::find_if(daq_modules_.begin(), daq_modules_.end(),
                           [&](const DAQModule *m)
                           { return m->getName() == module_name; });
    if (it != daq_modules_.end())
        return *it;
    return nullptr;
}

const Run *Experiment::getRun(const Int_t run_number) const
{
    auto it = std::find_if(runs_.begin(), runs_.end(),
                           [run_number](const Run *run)
                           { return run->getRunNumber() == run_number; });
    return (it != runs_.end()) ? *it : nullptr;
}

void Experiment::addDAQModule(DAQModule *module)
{
    daq_modules_.push_back(module);
}

void Experiment::removeDAQModule(const TString &module_name)
{
    auto it = std::remove_if(daq_modules_.begin(), daq_modules_.end(),
                             [module_name](const DAQModule *pmodule)
                             { return pmodule->getName() == module_name; });
    if (it != daq_modules_.end())
    {
        daq_modules_.erase(it, daq_modules_.end());
    }
}

void Experiment::addRun(Run *run)
{
    runs_.push_back(run);
}

void Experiment::removeRun(const Int_t run_number)
{
    auto it = std::remove_if(runs_.begin(), runs_.end(),
                             [run_number](const Run *prun)
                             { return prun->getRunNumber() == run_number; });
    if (it != runs_.end())
    {
        runs_.erase(it, runs_.end());
    }
}

void Experiment::printInfo() const
{
    std::cout << Form("Experiment %s (%s)", name_.Data(), file_name_.Data()) << std::endl;

    // DAQ Modules
    std::cout << "├── Modules" << std::endl;
    for (size_t i = 0; i < daq_modules_.size(); ++i)
    {
        const auto pmodule = daq_modules_[i];
        bool is_last_module = (i == daq_modules_.size() - 1);
        std::cout << "│   " << (is_last_module ? "└── " : "├── ");
        pmodule->printInfo();

        // Print detectors for this module, indented under the module
        const auto *detectors = pmodule->getDetectors();
        if (detectors && !detectors->empty())
        {
            for (size_t j = 0; j < detectors->size(); ++j)
            {
                bool is_last_detector = (j == detectors->size() - 1);
                // Indent to show detector is under its module
                std::cout << "│   " << (is_last_module ? "    " : "│   ");
                std::cout << (is_last_detector ? "    └── " : "    ├── ");
                (*detectors)[j]->printInfo();
            }
        }
    }

    // Runs
    std::cout << "└── Runs" << std::endl;
    for (size_t i = 0; i < runs_.size(); ++i)
    {
        std::cout << "    " << (i == runs_.size() - 1 ? "└── " : "├── ");
        runs_[i]->printInfo();
    }
}
