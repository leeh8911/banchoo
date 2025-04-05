/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include <iostream>

#include <nlohmann/json.hpp>

#include "app/app_factory.hpp"
#include "common/logger.hpp"

int main() noexcept
{
    std::ifstream ifs("config.json");
    nlohmann::json config = nlohmann::json::parse(ifs);

    BANCHOO_TRACE("Banchoo logger creating...");
    banchoo::Logger::init(config["log_level"],
                          config.value("log_file", "")); // 로거 초기화

    BANCHOO_TRACE("Banchoo server creating...");
    auto app = banchoo::app::AppFactory::create(config["app"]);

    BANCHOO_TRACE("Banchoo server running...");
    app->run(); // 서버 실행

    return 0;
}
