/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */                                                                                                                    \
#pragma once

#include <third_party/crow_all.h>

#include <memory>

#include "app/app_interface.hpp"
#include "repository/repository_interface.hpp"

namespace banchoo::app
{

class CrowApp : public IApp
{
  public:
    void configure() override;
    void run() override;

  private:
    crow::SimpleApp app_;
    std::unique_ptr<repository::IRepository> repo_;
};

} // namespace banchoo::app
