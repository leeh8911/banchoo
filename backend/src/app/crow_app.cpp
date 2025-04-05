/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include <crow_all.h>

#include <string>

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

    // 🔸 단일 Note 조회
    CROW_ROUTE(app_, "/notes/<int>")
        .methods("GET"_method)(
            [this](int id)
            {
                auto result = repo_->getNote(id);
                if (!result)
                    return crow::response(404);
                return crow::response(
                    json({{"id", result->id}, {"content", result->content}})
                        .dump());
            });

    // 🔸 전체 Note 조회
    CROW_ROUTE(app_, "/notes")
        .methods("GET"_method)(
            [this]()
            {
                auto notes = repo_->getAllNotes();
                json res = json::array();
                for (const auto &n : notes)
                    res.push_back({{"id", n.id}, {"content", n.content}});
                return crow::response(res.dump());
            });

    // 🔸 Memo
    CROW_ROUTE(app_, "/memos")
        .methods("POST"_method)(
            [this](const crow::request &req)
            {
                auto body = json::parse(req.body);
                note::Note n;
                n.content = body["content"];

                auto id = repo_->createMemo(n);
                return crow::response(
                    json({{"id", id}, {"content", n.content}}).dump());
            });

    CROW_ROUTE(app_, "/memos")
        .methods("GET"_method)(
            [this]()
            {
                auto memos = repo_->getAllMemos();
                json res = json::array();
                for (const auto &m : memos)
                    res.push_back({{"id", m.id}, {"content", m.content}});
                return crow::response(res.dump());
            });

    // 🔸 Task
    CROW_ROUTE(app_, "/tasks")
        .methods("POST"_method)(
            [this](const crow::request &req)
            {
                auto body = json::parse(req.body);
                note::Note n;
                n.content = body["content"];
                if (body.contains("status"))
                {
                    auto status = body["status"].get<std::string>();
                    if (status == "TODO")
                        n.status = note::NoteStatus::TODO;
                    else if (status == "DOING")
                        n.status = note::NoteStatus::DOING;
                    else if (status == "DONE")
                        n.status = note::NoteStatus::DONE;
                }
                if (body.contains("due_date"))
                {
                    auto due_date = body["due_date"].get<std::string>();
                    n.due_date = note::parse_time(due_date);
                }
                auto id = repo_->createTask(n);
                return crow::response(
                    json({{"id", id}, {"content", n.content}}).dump());
            });

    CROW_ROUTE(app_, "/tasks")
        .methods("GET"_method)(
            [this]()
            {
                auto tasks = repo_->getAllTasks();
                json res = json::array();
                for (const auto &t : tasks)
                    res.push_back({{"id", t.id}, {"content", t.content}});
                return crow::response(res.dump());
            });

    // 🔸 Event
    CROW_ROUTE(app_, "/events")
        .methods("POST"_method)(
            [this](const crow::request &req)
            {
                auto body = json::parse(req.body);
                note::Note n;
                n.content = body["content"];
                if (body.contains("start_date"))
                {
                    auto start_date = body["start_date"].get<std::string>();
                    n.start_date = note::parse_time(start_date);
                }
                if (body.contains("end_date"))
                {
                    auto end_date = body["end_date"].get<std::string>();
                    n.end_date = note::parse_time(end_date);
                }
                auto id = repo_->createEvent(n);
                return crow::response(
                    json({{"id", id}, {"content", n.content}}).dump());
            });

    CROW_ROUTE(app_, "/events")
        .methods("GET"_method)(
            [this]()
            {
                auto events = repo_->getAllEvents();
                json res = json::array();
                for (const auto &e : events)
                    res.push_back({{"id", e.id}, {"content", e.content}});
                return crow::response(res.dump());
            });

    // 🔸 Note 수정
    CROW_ROUTE(app_, "/notes/<int>")
        .methods("PUT"_method)(
            [this](const crow::request &req, int id)
            {
                auto body = json::parse(req.body);
                note::Note n;
                n.id = id;
                n.content = body["content"];
                bool ok = repo_->updateNote(n);
                return crow::response(ok ? 200 : 404);
            });

    // 🔸 Note 삭제
    CROW_ROUTE(app_, "/notes/<int>")
        .methods("DELETE"_method)(
            [this](int id)
            {
                bool ok = repo_->deleteNote(id);
                return crow::response(ok ? 200 : 404);
            });
}

void CrowApp::run()
{
    app_.port(this->getPort())
        .bindaddr(this->getBindAddr())
        .multithreaded()
        .run();
}

} // namespace banchoo::app
