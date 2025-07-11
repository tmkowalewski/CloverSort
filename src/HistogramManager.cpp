// Standard C++ includes
#include <iostream>
#include <string>

// ROOT includes

// Project includes
#include "HistogramManager.hpp"
#include "Experiment.hpp"
#include "DAQModule.hpp"
#include "Detector.hpp"

HistogramManager::HistogramManager()
{
    // Constructor implementation
}

HistogramManager::HistogramManager(Experiment *pexperiment)
{
    InitFromExperiment(pexperiment);
}

HistogramManager::~HistogramManager()
{
    // Destructor implementation
}

void HistogramManager::InitFromExperiment(Experiment *pexperiment)
{
    for (const auto &pdaq_module : pexperiment->GetDAQModules())
    {
        auto module_name = pdaq_module->GetName();

        // Create module-specific histograms (e.g., module_timestamp)
        for (const auto &filter : pdaq_module->GetFilters())
        {
            if (filter == "module_timestamp")
            {
                std::string hist_name = Form("%s.%s", module_name.c_str(), filter.c_str());
                std::string hist_title = Form("%s %s", module_name.c_str(), filter.c_str());
                AddHistogram(hist_name, hist_title, DAQ_BINS, 0, DAQ_BINS - 1, module_name.c_str(), filter.c_str(), 0);
            }
            if (filter == "trigger_time")
            {
                // Create a histogram for trigger_time with a different binning
                std::string hist_name_trigger0 = Form("%s.trigger_time[0]", module_name.c_str());
                std::string hist_title_trigger0 = Form("%s Trigger 0 Time", module_name.c_str());
                std::string hist_name_trigger1 = Form("%s.trigger_time[1]", module_name.c_str());
                std::string hist_title_trigger1 = Form("%s Trigger 1 Time", module_name.c_str());
                AddHistogram(hist_name_trigger0, hist_title_trigger0, DAQ_BINS, 0, DAQ_BINS - 1, module_name.c_str(), filter.c_str(), 0);
                AddHistogram(hist_name_trigger1, hist_title_trigger1, DAQ_BINS, 0, DAQ_BINS - 1, module_name.c_str(), filter.c_str(), 1);
            }
        }

        // Create channel-specific histograms for all other filters
        for (const auto &pdetector : pdaq_module->GetDetectors())
        {
            auto detector_name = pdetector->GetName();

            for (const auto &channel : pdetector->GetChannels()) // Assuming 'channel' is a 0-indexed integer ID
            {
                for (const auto &filter : pdaq_module->GetFilters())
                {
                    if (filter != "module_timestamp" && filter != "trigger_time") // Exclude module_timestamp, as it's handled above
                    {
                        // Assuming 'channel' is a 0-indexed integer ID from the collection pdetector->GetChannels()
                        // and we want a 1-indexed crystal number for the histogram name.
                        // This calculation maps a 0-indexed 'channel' value to a 1-indexed 'crystal_num'.
                        Int_t crystal_num = channel % pdetector->GetChannels().size() + 1;
                        std::string hist_name = Form("%sE%i.%s", detector_name.c_str(), crystal_num, filter.c_str());
                        std::string hist_title = Form("%sE%i %s", detector_name.c_str(), crystal_num, filter.c_str());
                        AddHistogram(hist_name, hist_title, DAQ_BINS, 0, DAQ_BINS - 1, detector_name.c_str(), filter.c_str(), crystal_num);
                    }
                }
            }
        }
    }
}

void HistogramManager::AddHistogram(const std::string &name, const std::string &title, const Int_t nbinsx, const Double_t &xlow, const Double_t &xup, const std::string &owner, const std::string &filter, const UInt_t index)
{
    // Create a histogram path based on the owner, filter, and index
    auto hist = new ROOT::TThreadedObject<TH1D>(name.c_str(), title.c_str(), nbinsx, xlow, xup);
    histogram_map_[owner][filter][index] = hist;
}

HistogramManager::HistogramPtrMap HistogramManager::MakeHistPtrMap()
{
    HistogramPtrMap hist_ptr_map;

    for (const auto &owner_pair : histogram_map_)
    {
        auto owner(owner_pair.first);
        const auto &filter_map = owner_pair.second;

        for (const auto &filter_pair : filter_map)
        {
            auto filter(filter_pair.first);
            const auto &index_map = filter_pair.second;

            for (const auto &index_pair : index_map)
            {
                const auto index = index_pair.first;
                auto *hist = index_pair.second;

                hist_ptr_map[owner][filter][index] = hist->Get();
            }
        }
    }

    return hist_ptr_map;
}

void HistogramManager::PrintInfo()
{
    struct PrintNode
    {
        std::map<TString, std::unique_ptr<PrintNode>> children; // Sub-nodes (directories)
        std::vector<ROOT::TThreadedObject<TH1D> *> histograms;  // Histograms at this node's level

        PrintNode() = default; // Default constructor
    };

    std::cout << Form("HistogramManager %s", name_.c_str()) << std::endl;

    // 1. Build a tree representation from histogram_map_
    auto root_node = std::make_unique<PrintNode>(); // Conceptual root, its children are the top-level items

    // 1. Build a tree representation from histogram_map_ (new nested unordered_map)
    for (const auto &owner_pair : histogram_map_)
    {
        const TString owner(owner_pair.first);
        const auto &filter_map = owner_pair.second;

        for (const auto &filter_pair : filter_map)
        {
            const TString filter(filter_pair.first);
            const auto &index_map = filter_pair.second;

            for (const auto &index_pair : index_map)
            {
                const UInt_t index = index_pair.first;
                auto *hist = index_pair.second;

                // Walk/create the path: owner → filter → index
                PrintNode *current = root_node.get();
                for (const TString &segment : {owner,
                                               filter,
                                               TString(Form("%u", index))})
                {
                    auto it = current->children.find(segment);
                    if (it == current->children.end())
                    {
                        current->children[segment] = std::make_unique<PrintNode>();
                        it = current->children.find(segment);
                    }
                    current = it->second.get();
                }

                // Attach the histogram to the leaf node
                current->histograms.push_back(hist);
            }
        }
    }

    // 2. Define a recursive lambda function to print the tree
    std::function<void(const TString &, const PrintNode *, const std::string &, bool)> print_node_recursive;
    print_node_recursive =
        [&](const TString &node_name,       // Name of the current directory/node to print
            const PrintNode *node_data,     // The data of the current node (its histograms and children)
            const std::string &line_prefix, // Prefix for the current node's line (e.g., "│   ")
            bool is_last_sibling)           // True if this node is the last among its siblings
    {
        // Print the current node's (directory) name
        std::cout << line_prefix;
        std::cout << (is_last_sibling ? "└── " : "├── ");
        std::cout << node_name.Data() << std::endl;

        // Determine the prefix for items (histograms/subdirectories) listed under this node
        std::string children_prefix = line_prefix + (is_last_sibling ? "    " : "│   ");

        // Combine histograms and subdirectories to correctly determine the "last item" for connector symbols
        size_t num_histograms = node_data->histograms.size();
        size_t num_subdirs = node_data->children.size();
        size_t total_items_in_node = num_histograms + num_subdirs;
        size_t items_processed_so_far = 0;

        // Print histograms at this node's level
        for (const auto &hist_obj_ptr : node_data->histograms)
        {
            items_processed_so_far++;
            bool is_this_hist_the_last_item = (items_processed_so_far == total_items_in_node);

            std::cout << children_prefix;
            std::cout << (is_this_hist_the_last_item ? "└── " : "├── ");
            // Assuming hist_obj_ptr->Get() returns TH1D* or similar with GetName() and GetTitle()
            std::cout << hist_obj_ptr->Get()->GetName() << Form(" (%s)", hist_obj_ptr->Get()->GetTitle()) << std::endl;
        }

        // Recursively print subdirectories
        // std::map iterates in key-sorted order, ensuring consistent output for subdirectories
        auto subdir_iterator = node_data->children.begin();
        while (subdir_iterator != node_data->children.end())
        {
            items_processed_so_far++;
            bool is_this_subdir_the_last_item = (items_processed_so_far == total_items_in_node);

            const TString &subdir_name = subdir_iterator->first;
            const std::unique_ptr<PrintNode> &subdir_node_ptr = subdir_iterator->second;

            print_node_recursive(subdir_name, subdir_node_ptr.get(), children_prefix, is_this_subdir_the_last_item);

            ++subdir_iterator;
        }
    };

    // 3. Start printing from the children of the conceptual root node
    size_t total_top_level_nodes = root_node->children.size();
    size_t top_level_nodes_processed = 0;

    // std::map iterates in key-sorted order, ensuring consistent output for top-level directories
    auto top_level_iterator = root_node->children.begin();
    while (top_level_iterator != root_node->children.end())
    {
        top_level_nodes_processed++;
        bool is_last_top_level_node = (top_level_nodes_processed == total_top_level_nodes);

        const TString &node_name = top_level_iterator->first;
        const std::unique_ptr<PrintNode> &node_ptr = top_level_iterator->second;

        print_node_recursive(node_name, node_ptr.get(), "", is_last_top_level_node); // Initial line_prefix is empty

        ++top_level_iterator;
    }
}