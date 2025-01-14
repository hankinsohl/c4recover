// Copyright (c) 2025 By David "Hankinsohl" Hankins.
// This software is licensed under the terms of the MIT License.
// Created by Hankinsohl on 1/11/2025.

#include <array>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <c4lib/include/node-attributes.hpp>
#include <filesystem>
#include <src/constants.hpp>
#include <src/exceptions.hpp>
#include <src/narrow.hpp>
#include <src/util.hpp>
#include <string>

using namespace std::string_literals;
namespace bpt = boost::property_tree;
namespace cpt = c4lib::property_tree;

namespace c4recover {

void get_config(std::unordered_map<std::string, std::string>& config)
{
    // Load the XML config file into a boost property tree.
    bpt::ptree tree;
    bpt::read_xml(constants::config_file, tree);

    for (const bpt::ptree& config_element{tree.get_child("config")}; const auto& option_element : config_element) {
        if (option_element.first != "setting") {
            throw Xml_error{text::bad_config_setting};
        }
        const std::string option_name{option_element.second.get("<xmlattr>.name", "")};
        const std::string option_value{option_element.second.get("<xmlattr>.value", "")};

        // Convert setting name to uppercase to facilitate lookup.
        std::string uc{option_name};
        std::ranges::transform(uc, uc.begin(), [](unsigned char c) { return gsl::narrow<char>(std::toupper(c)); });
        config[uc] = option_value;
    }
}

std::string get_recovered_save_name(const std::string& save_name)
{
    std::filesystem::path recovered_save_filename{std::filesystem::path{"recovered_"}};
    recovered_save_filename += std::filesystem::path{save_name}.filename();
    std::filesystem::path recovered_save_path{std::filesystem::path{save_name}};
    recovered_save_path.replace_filename(recovered_save_filename);
    return recovered_save_path.string();
}

std::string get_save_name(int argc, char** argv)
{
    if (argc != 2) {
        throw Usage_error(text::bad_usage);
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    return argv[1];
}

void remove_lma(bpt::ptree& root)
{
    // To remove LMA-protection, set the following strings located in GameHeader to length 0.
    //     LockModifiedAssetsText
    //     LmaMd5_1
    //     LmaMd5_2
    //     LmaMd5_3
    //     LmaMd5_4
    constexpr std::array lma_string_fields{"LockModifiedAssetsText", "LmaMd5_1", "LmaMd5_2", "LmaMd5_3", "LmaMd5_4"};
    for (const auto& lma : lma_string_fields) {
        root.put("Savegame.GameHeader."s + lma + "." + cpt::nn_attributes + "." + cpt::nn_data, "");
    }
}

} // namespace c4recover
