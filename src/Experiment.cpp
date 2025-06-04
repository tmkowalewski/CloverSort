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
Experiment::Experiment(const TString name, const TString file_name)
    : name_(name), file_name_(file_name),
      daq_modules_(), runs_()
{
    // Set the file and check if it exists
    std::ifstream config_file(file_name.Data());
    if (!config_file.is_open())
    {
        std::cerr << "Error: Could not open configuration file: " << file_name.Data() << std::endl;
        return;
    }

    // Read the configuration file
    std::string line;
    std::string current_section;

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
        else if (current_section == "Modules")
        {
            // Format: module_name    module_type   filters...]
            std::string module_name, module_type;
            if (!(iss >> module_name >> module_type))
                continue;

            // Check if module_name is in the list of valid module names
            if (std::find(DAQModule::VALID_MODULE_TYPES_.begin(), DAQModule::VALID_MODULE_TYPES_.end(), module_name) == DAQModule::VALID_MODULE_TYPES_.end())
            {
                std::cerr << "Error: Unsupported module type: " << module_type << std::endl;
                throw std::runtime_error("Unsupported module name: " + module_type);
            }
            // Create a DAQModule object and parse filters
            DAQModule module(module_name, module_type);
            std::string filter;
            while (iss >> filter)
            {
                if (filter == "default")
                {
                    module.generateDefaultFilters();
                }
                else
                {
                    // Add the filter to the module
                    module.addFilter(filter);
                }
                module.addFilter(filter);
            }
            // Add the module to the list of DAQ modules
            daq_modules_.push_back(module);
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

            // Create a Detector object and add it to the appropriate DAQModule
            if (!daq_modules_.empty())
            {
                Detector detector(detector_name, detector_type, detector_channels_parsed);
                daq_modules_.back().addDetector(detector);
            }
        }
        // Handle Run definitions
        else if (current_section == "Runs")
        {
            // Format: run_number    run_description    run_type    file_name
            std::string run_numbers, run_description, run_type, file_name, tree_name;
            if (!(iss >> run_numbers >> run_description >> run_type >> file_name >> tree_name))
                continue;
            // Parse the run numbers and file name. If it contains '###' we need to replace that with the proper run number in 3 digit format
            std::vector<Int_t> run_numbers_parsed = parseNumberString(run_numbers);
            if (file_name.find("###") != std::string::npos)
            {
                for (Int_t run_number : run_numbers_parsed)
                {
                    // Format the filename correctly
                    std::ostringstream oss;
                    oss << std::setw(3) << std::setfill('0') << run_number;
                    file_name = file_name.replace(file_name.find("###"), 3, oss.str());

                    // Replace '###' in the file name with the run number in 3-digit format
                    std::string formatted_file_name = file_name;
                    formatted_file_name = formatted_file_name.replace(formatted_file_name.find("###"), 3, oss.str());

                    // Create a Run object
                    Run run(run_number, file_name, tree_name, run_description, run_type);

                    runs_.push_back(run);
                }
            }
        }
    }

    printInfo();
}

// Destructor
Experiment::~Experiment()
{
    // Clean up resources if needed
}

const TString &Experiment::getName() const
{
    return name_;
}

const DAQModule *Experiment::getDAQModule(const TString module_name) const
{
    for (const auto &module : daq_modules_)
    {
        if (module.getName() == module_name)
        {
            return &module;
        }
    }
    return nullptr; // Return nullptr if the module is not found
}

const std::vector<DAQModule> *Experiment::getDAQModules() const
{
    return &daq_modules_;
}

const Run *Experiment::getRun(const Int_t runNumber) const
{
    for (const auto &run : runs_)
    {
        if (run.getRunNumber() == runNumber)
        {
            return &run;
        }
    }
    return nullptr; // Return nullptr if the run is not found
}

const std::vector<Run> *Experiment::getRuns() const
{
    return &runs_;
}

void Experiment::addDAQModule(const DAQModule &module)
{
    daq_modules_.push_back(module);
}

void Experiment::removeDAQModule(const TString &module_name)
{
    auto it = std::remove_if(daq_modules_.begin(), daq_modules_.end(),
                             [&module_name](const DAQModule &module)
                             { return module.getName() == module_name; });
    if (it != daq_modules_.end())
    {
        daq_modules_.erase(it, daq_modules_.end());
    }
}

void Experiment::addRun(const Run &run)
{
    runs_.push_back(run);
}

void Experiment::removeRun(const Int_t &run_number)
{
    auto it = std::remove_if(runs_.begin(), runs_.end(),
                             [&run_number](const Run &run)
                             { return run.getRunNumber() == run_number; });
    if (it != runs_.end())
    {
        runs_.erase(it, runs_.end());
    }
}

void Experiment::printInfo() const
{
    std::cout << "Experiment Name: " << name_.Data() << std::endl;
    std::cout << "File Name: " << file_name_.Data() << std::endl;

    std::cout << "DAQ Modules:" << std::endl;
    for (const auto &module : daq_modules_)
    {
        module.printInfo();
    }

    std::cout << "Runs:" << std::endl;
    for (const auto &run : runs_)
    {
        run.printInfo();
    }
}
