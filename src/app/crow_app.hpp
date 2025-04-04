/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */                                                                                                                    \
#pragma once

#include <memory>

#include <crow_all.h>
#include <nlohmann/json.hpp>

#include "app/app_interface.hpp"
#include "repository/base_repository.hpp"

namespace banchoo::app
{

class CrowApp : public IApp
{
  public:
    void configure(const nlohmann::json &config) override;
    void run() override;

  private:
    crow::SimpleApp app_;
    std::shared_ptr<repository::BaseRepository> repo_;
};

} // namespace banchoo::app
