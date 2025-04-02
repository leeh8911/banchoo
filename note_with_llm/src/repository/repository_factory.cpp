/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include "repository/repository_factory.hpp"

#include <memory>
#include <string_view>

#include "repository/inmemory_repository.hpp"
#include "repository/repository_interface.hpp"

namespace banchoo::repository
{

std::shared_ptr<IRepository> RepositoryFactory::create(std::string_view type)
{
    if (type == "inmemory")
    {
        return std::make_shared<InMemoryRepository>();
    }
    else
    {
        throw std::invalid_argument("Invalid repository type");
    }
    return nullptr;
}

} // namespace banchoo::repository