/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include <third_party/nlohmann/json.hpp>

#include "app/app_factory.hpp"

int main()
{
    std::ifstream ifs("config.json");
    nlohmann::json config = nlohmann::json::parse(ifs);

    auto app = banchoo::app::AppFactory::create(config["app"]);

    app->run(); // 서버 실행

    return 0;
}
