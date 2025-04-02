/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */                                                                                                                    \
#pragma once

#include <memory>
#include <string_view>

#include "repository/repository_interface.hpp"

namespace banchoo::repository
{
class RepositoryFactory
{
  public:
    static std::shared_ptr<IRepository> create(std::string_view type);
};
} // namespace banchoo::repository