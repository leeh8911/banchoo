/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#include <third_party/crow_all.h>
#include <third_party/nlohmann/json.hpp>

#include "app/crow_app.hpp"
#include "note/note.hpp"
#include "repository/inmemory_repository.hpp"

using json = nlohmann::json;

namespace banchoo::app
{

void CrowApp::configure()
{
    repo_ = std::make_unique<repository::InMemoryRepository>();

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
    app_.port(18080).bindaddr("0.0.0.0").multithreaded().run();
}

} // namespace banchoo::app
