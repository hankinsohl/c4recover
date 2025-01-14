// Copyright (c) 2025 By David "Hankinsohl" Hankins.
// This software is licensed under the MIT License.
// Created by Hankinsohl on 1/10/2025.

#include <boost/property_tree/ptree.hpp>
#include <exception>
#include <include/c4lib.hpp>
#include <iostream>
#include <src/exceptions.hpp>
#include <src/text.hpp>
#include <src/util.hpp>
#include <string>

namespace bpt = boost::property_tree;

int main(int argc, char* argv[])
{
    int rc{0};
    try {
        std::cout << c4recover::banner() << '\n';
        const std::string save{c4recover::get_save_name(argc, argv)};
        std::unordered_map<std::string, std::string> config;
        c4recover::get_config(config);

        config["SCHEMA"] = "BTS.schema";

        bpt::ptree ptree;
        std::cout << c4recover::text::reading_save_from << " " << save << '\n';
        c4lib::read_save(ptree, save, config);

        std::cout << c4recover::text::removing_lma << '\n';
        c4recover::remove_lma(ptree);

        const std::string modified_save{c4recover::get_recovered_save_name(save)};
        std::cout << c4recover::text::writing_recovered_save_to << " " << modified_save << '\n';
        c4lib::write_save(ptree, modified_save, config);

        std::cout << c4recover::text::finished << ".\n";
    }
    catch (const c4recover::Usage_error& ex) {
        std::cerr << '\n' << ex.what() << '\n';
        c4recover::display_usage();
        rc = -1;
    }
    catch (const std::exception& ex) {
        std::cerr << '\n' << ex.what() << '\n';
        rc = -1;
    }

    return rc;
}
