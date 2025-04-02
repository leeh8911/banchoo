/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include "app/app_factory.hpp"

#include <memory>
#include <string_view>

#include <third_party/nlohmann/json.hpp>

#include "app/app_interface.hpp"
#include "app/crow_app.hpp"

namespace banchoo::app
{

std::shared_ptr<IApp> AppFactory::create(const nlohmann::json &config)
{
    auto type = config["type"].get<std::string>();

    std::shared_ptr<IApp> app = nullptr;

    if (type == "crow")
    {
        app = std::make_shared<CrowApp>();
    }
    else
    {
        throw std::invalid_argument("Invalid app type");
    }

    app->configure(config);

    return app;
}

} // namespace banchoo::app