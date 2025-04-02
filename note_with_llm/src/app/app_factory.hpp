/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#pragma once

#include <memory>
#include <string_view>

#include "app/app_interface.hpp"
#include "app/crow_app.hpp"

namespace banchoo::app
{
class AppFactory
{
  public:
    static std::shared_ptr<IApp> create(std::string_view type);
};
} // namespace banchoo::app
