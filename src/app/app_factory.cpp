/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include "app/app_factory.hpp"

#include <memory>
#include <string>
#include <string_view>

#include <nlohmann/json.hpp>

#include "app/base_app.hpp"
#include "app/crow_app.hpp"
#include "common/logger.hpp"

namespace banchoo::app
{

std::shared_ptr<BaseApp> AppFactory::create(const nlohmann::json &config)
{
    auto type = config["type"].get<std::string>();
    BANCHOO_TRACE("Create {} app", type);

    std::shared_ptr<BaseApp> app = nullptr;

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
