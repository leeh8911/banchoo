/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include <crow_all.h>
#include <nlohmann/json.hpp>

#include "app/crow_app.hpp"
#include "note/note.hpp"
#include "repository/repository_factory.hpp"

using json = nlohmann::json;

namespace banchoo::app
{

void CrowApp::configure(const nlohmann::json &config)
{
    repo_ = repository::RepositoryFactory::create(config["repository"]);

    this->setPort(config["port"].get<uint32_t>());
    this->setBindAddr(config["bindaddr"].get<std::string>());

    CROW_ROUTE(app_, "/notes").methods("POST"_method)([this](const crow::request &req) {
        auto body = json::parse(req.body);
        note::Note n;
        n.content = body["content"];
        auto id = repo_->createNote(n);
        json res = {{"id", id}, {"content", n.content}};
        return crow::response{res.dump()};
    });

    CROW_ROUTE(app_, "/notes/<int>").methods("GET"_method)([this](int id) {
        auto result = repo_->getNote(id);
        if (!result)
            return crow::response(404);
        json res = {{"id", result->id}, {"content", result->content}};
        return crow::response{res.dump()};
    });
}

void CrowApp::run()
{
    app_.port(this->getPort()).bindaddr(this->getBindAddr()).multithreaded().run();
}

} // namespace banchoo::app
