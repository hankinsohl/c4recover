// Copyright (c) 2025 By David "Hankinsohl" Hankins.
// This software is licensed under the MIT License.
// Created by Hankinsohl on 1/10/2025.

#pragma once

#include <boost/property_tree/ptree_fwd.hpp>
#include <c4recover-version.hpp>
#include <format>
#include <iostream>
#include <src/text.hpp>
#include <string>
#include <unordered_map>

namespace c4recover {

inline std::string banner()
{
    return std::format("{} {} {}", text::exe_name, text::version, constants::c4recover_version);
}

inline void display_usage()
{
    std::cout << std::format("{}: {} <{}>.CivBeyondSwordSave\n", text::usage, text::exe_name, text::save_name);
}

void get_config(std::unordered_map<std::string, std::string>& config);

std::string get_recovered_save_name(const std::string& save_name);

std::string get_save_name(int argc, char** argv);

void remove_lma(boost::property_tree::ptree& root);

} // namespace c4recover
