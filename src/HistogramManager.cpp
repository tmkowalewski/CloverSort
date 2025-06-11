#include <iostream>
#include "HistogramManager.hpp"
#include "Experiment.hpp"
#include "DAQModule.hpp"
#include "Detector.hpp"

HistogramManager::HistogramManager()
{
    // Constructor implementation
}

HistogramManager::HistogramManager(Experiment *experiment)
{
    InitFromExperiment(experiment);
}

HistogramManager::~HistogramManager()
{
    // Destructor implementation
    for (auto &pair : histogram_map_)
    {
        for (auto &histogram : pair.second)
        {
            delete histogram; // Clean up dynamically allocated histograms
        }
    }
    histogram_map_.clear(); // Clear the map
}

void HistogramManager::InitFromExperiment(Experiment *experiment)
{
    for (const auto &pdaq_module : experiment->GetDAQModules())
    {
        // Create module-specific histograms (e.g., module_timestamp)
        for (const auto &filter : pdaq_module->GetFilters())
        {
            if (filter == "module_timestamp" || filter == "trigger_time")
            {
                std::vector<TString> histogram_path = {pdaq_module->GetName()};
                TString hist_name = Form("%s.%s", pdaq_module->GetName().Data(), filter.Data());
                TString hist_title = Form("%s %s", pdaq_module->GetName().Data(), filter.Data());
                AddHistogram(hist_name, hist_title, DAQ_BINS, 0, DAQ_BINS - 1, histogram_path);
            }
        }

        // Create channel-specific histograms for all other filters
        for (const auto &pdetector : pdaq_module->GetDetectors())
        {
            for (const auto &channel : pdetector->GetChannels()) // Assuming 'channel' is a 0-indexed integer ID
            {
                for (const auto &filter : pdaq_module->GetFilters())
                {
                    if (filter != "module_timestamp" && filter != "trigger_time") // Exclude module_timestamp, as it's handled above
                    {
                        std::vector<TString> histogram_path = {pdaq_module->GetName(), pdetector->GetName()};
                        // Assuming 'channel' is a 0-indexed integer ID from the collection pdetector->GetChannels()
                        // and we want a 1-indexed crystal number for the histogram name.
                        // This calculation maps a 0-indexed 'channel' value to a 1-indexed 'crystal_num'.
                        Int_t crystal_num = channel % pdetector->GetChannels().size() + 1;
                        TString hist_name = Form("%sE%i.%s", pdetector->GetName().Data(), crystal_num, filter.Data());
                        TString hist_title = Form("%sE%i %s", pdetector->GetName().Data(), crystal_num, filter.Data());
                        AddHistogram(hist_name, hist_title, DAQ_BINS, 0, DAQ_BINS - 1, histogram_path);
                    }
                }
            }
        }
    }
}

void HistogramManager::AddHistogram(const TString &name, const TString &title, const Int_t nbinsx, const Double_t &xlow, const Double_t &xup, std::vector<TString> &histogram_path)
{
    auto hist = new ROOT::TThreadedObject<TH1D>(name, title, nbinsx, xlow, xup);
    histogram_map_[histogram_path].push_back(hist);
}

void HistogramManager::RemoveHistogram(const std::vector<TString> &histogram_path, const TString &name)
{
    std::remove_if(histogram_map_[histogram_path].begin(), histogram_map_[histogram_path].end(),
                   [&name](ROOT::TThreadedObject<TH1D> *hist)
                   { return hist->Get()->GetName() == name; });
}

HistogramManager::HistogramPtrMap HistogramManager::MakeHistPtrMap()
{
    HistogramPtrMap hist_ptr_map;
    for (auto &pair : histogram_map_)
    {
        const auto &histogram_path = pair.first;
        auto &histograms = pair.second;
        std::vector<std::shared_ptr<TH1D>> hist_ptrs;
        for (auto &histogram : histograms)
        {
            hist_ptrs.push_back(histogram->Get());
        }
        hist_ptr_map[histogram_path] = hist_ptrs;
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

    std::cout << Form("HistogramManager %s", name_.Data()) << std::endl;

    // 1. Build a tree representation from histogram_map_
    auto root_node = std::make_unique<PrintNode>(); // Conceptual root, its children are the top-level items

    for (const auto &pair : histogram_map_)
    {
        const std::vector<TString> &path = pair.first;
        const std::vector<ROOT::TThreadedObject<TH1D> *> &hists_for_path = pair.second;

        PrintNode *current_treenode = root_node.get();
        for (const TString &segment : path)
        {
            // std::map::operator[] creates if not exists, but here we need to manage unique_ptr
            if (current_treenode->children.find(segment) == current_treenode->children.end())
            {
                current_treenode->children[segment] = std::make_unique<PrintNode>();
            }
            current_treenode = current_treenode->children[segment].get();
        }
        // Add histograms to the target node
        current_treenode->histograms.insert(
            current_treenode->histograms.end(),
            hists_for_path.begin(),
            hists_for_path.end());
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