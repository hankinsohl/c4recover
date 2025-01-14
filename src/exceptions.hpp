// Copyright (c) 2025 By David "Hankinsohl" Hankins.
// This software is licensed under the terms of the MIT License.
// Created by Hankinsohl on 1/11/2025.

#pragma once

#include <stdexcept>

namespace c4recover {

class Usage_error : public std::logic_error {
public:
    explicit Usage_error(const std::string& what)
        : logic_error{what}
    {}
};

class Xml_error : public std::logic_error {
public:
    explicit Xml_error(const std::string& what)
        : logic_error{what}
    {}
};

} // namespace c4recover
