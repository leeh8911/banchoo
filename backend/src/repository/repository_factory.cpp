/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include "repository/repository_factory.hpp"

#include <memory>
#include <string>
#include <string_view>

#include <nlohmann/json.hpp>

#include "repository/base_repository.hpp"
#include "repository/inmemory_repository.hpp"
#include "repository/sqlite_repository.hpp"

namespace banchoo::repository
{

std::shared_ptr<BaseRepository>
RepositoryFactory::create(const nlohmann::json &config)
{
    auto type = config["type"].get<std::string>();

    if (type == "inmemory")
    {
        return std::make_shared<InMemoryRepository>(config);
    }
    if (type == "sqlite")
    {
        return std::make_shared<SqliteRepository>(config);
    }
    else
    {
        throw std::invalid_argument("Invalid repository type");
    }
    return nullptr;
}

} // namespace banchoo::repository
