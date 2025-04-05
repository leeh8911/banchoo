/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#pragma once

#include <crow_all.h>

#include <memory>

#include <nlohmann/json.hpp>

#include "app/base_app.hpp"
#include "app/crow_cors.hpp"
#include "repository/base_repository.hpp"

namespace banchoo::app
{

class CrowApp : public BaseApp
{
 public:
    void configure(const nlohmann::json &config) override;
    void run() override;

 private:
    crow::App<Cors> app_;
    std::shared_ptr<repository::BaseRepository> repo_;
};

} // namespace banchoo::app
