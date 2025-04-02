/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include "app/app_factory.hpp"

#include <memory>
#include <string_view>

#include "app/app_interface.hpp"
#include "app/crow_app.hpp"

namespace banchoo::app
{

std::shared_ptr<IApp> AppFactory::create(std::string_view type)
{
    if (type == "crow")
    {
        return std::make_shared<CrowApp>();
    }
    else
    {
        throw std::invalid_argument("Invalid app type");
    }
    return nullptr;
}

} // namespace banchoo::app