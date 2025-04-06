/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#pragma once

#include "repository/sqlite_repository.hpp"

#include <mutex>
#include <unordered_map>
#include <vector>

#include "repository/base_repository.hpp"

namespace banchoo::repository
{

SqliteRepository::SqliteRepository(const nlohmann::json &config)
{
    std::string db_path = config["db_path"].get<std::string>();
    if (sqlite3_open(db_path.c_str(), &db_) != SQLITE_OK)
    {
        throw std::runtime_error("Failed to open database");
    }
    initializeDatabase();
}

SqliteRepository::~SqliteRepository()
{
    sqlite3_close(db_);
}

void SqliteRepository::initializeDatabase() const
{
    const char *sql = R"(
        CREATE TABLE IF NOT EXISTS notes (
            id INTEGER PRIMARY KEY,
            title TEXT,
            content TEXT,
            type TEXT
        );
    )";

    char *errMsg = nullptr;
    if (sqlite3_exec(db_, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::string error = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error("Failed to create table: " + error);
    }
}

note::Id SqliteRepository::createNote(const note::Note &note)
{
    const char *sql =
        "INSERT INTO notes (title, content, type) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare insert statement");

    sqlite3_bind_text(stmt, 1, note.title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, note.content.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, note.type.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute insert");
    }

    note::Id new_id = static_cast<note::Id>(sqlite3_last_insert_rowid(db_));
    sqlite3_finalize(stmt);
    return new_id;
}

std::optional<note::Note> SqliteRepository::getNote(note::Id id) const
{
    const char *sql =
        "SELECT id, title, content, type FROM notes WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return std::nullopt;

    sqlite3_bind_int(stmt, 1, static_cast<int>(id));

    note::Note note;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        note.id = sqlite3_column_int(stmt, 0);
        note.title =
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        note.content =
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        note.type =
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        sqlite3_finalize(stmt);
        return note;
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}

std::vector<note::Note> SqliteRepository::getAllNotes() const
{
    return queryNotesByType(""); // 빈 문자열이면 모두 가져옴
}

std::vector<note::Note> SqliteRepository::getAllMemos() const
{
    return queryNotesByType("memo");
}

std::vector<note::Note> SqliteRepository::getAllTasks() const
{
    return queryNotesByType("task");
}

std::vector<note::Note> SqliteRepository::getAllEvents() const
{
    return queryNotesByType("event");
}

std::vector<note::Note>
SqliteRepository::queryNotesByType(const std::string &type) const
{
    std::vector<note::Note> notes;
    std::stringstream ss;
    ss << "SELECT id, title, content, type FROM notes";
    if (!type.empty())
        ss << " WHERE type = ?";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, ss.str().c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK)
        return notes;

    if (!type.empty())
        sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        note::Note note;
        note.id = sqlite3_column_int(stmt, 0);
        note.title =
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        note.content =
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        note.type =
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        notes.push_back(note);
    }

    sqlite3_finalize(stmt);
    return notes;
}

bool SqliteRepository::updateNote(const note::Note &note)
{
    const char *sql =
        "UPDATE notes SET title = ?, content = ?, type = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, note.title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, note.content.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, note.type.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, static_cast<int>(note.id));

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool SqliteRepository::deleteNote(note::Id id)
{
    const char *sql = "DELETE FROM notes WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, static_cast<int>(id));
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}
} // namespace banchoo::repository
