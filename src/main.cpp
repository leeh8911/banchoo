/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include <nlohmann/json.hpp>

#include "app/app_factory.hpp"
#include "common/logger.hpp"

int main() noexcept
{
    std::ifstream ifs("config.json");
    nlohmann::json config = nlohmann::json::parse(ifs);

    banchoo::Logger::init(config["log_level"]); // 로거 초기화

    auto app = banchoo::app::AppFactory::create(config["app"]);

    app->run(); // 서버 실행

    return 0;
}
