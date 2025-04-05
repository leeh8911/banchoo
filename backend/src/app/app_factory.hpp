/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#pragma once

#include <memory>
#include <string_view>

#include <nlohmann/json.hpp>

#include "base_app.hpp"
#include "crow_app.hpp"

namespace banchoo::app
{
class AppFactory
{
 public:
    static std::shared_ptr<BaseApp> create(const nlohmann::json &config);
};
} // namespace banchoo::app
