/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include "repository/sqlite_repository.hpp"

#include <filesystem>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>
#include <sqlite/sqlite3.h>

#include "repository/base_repository.hpp"

namespace banchoo::repository
{

SqliteRepository::SqliteRepository(const nlohmann::json &config) : db_(nullptr)
{
    std::string db_path = config["db_path"].get<std::string>();
    if (db_path.empty())
    {
        throw std::invalid_argument("Database path is empty");
    }

    std::filesystem::path db_file_path(db_path);
    auto dir = db_file_path.parent_path();
    if (!dir.empty() && !std::filesystem::exists(dir))
    {
        std::filesystem::create_directories(dir);
    }

    if (sqlite3_open(db_path.c_str(), &db_) != SQLITE_OK)
    {
        throw std::runtime_error("Failed to open database");
    }

    this->initializeDatabase();
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
            type TEXT NOT NULL,
            content TEXT NOT NULL,
            created_at TEXT NOT NULL,
            updated_at TEXT NOT NULL,
            status TEXT,
            due_date TEXT,
            start_date TEXT,
            end_date TEXT
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
    const char *sql = R"(
        INSERT INTO notes (type, content, created_at, updated_at, status, due_date, start_date, end_date)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare insert");

    sqlite3_bind_text(
        stmt, 1, to_string(note.type).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, note.content.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,
                      3,
                      note::to_string(note.created_at).c_str(),
                      -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,
                      4,
                      note::to_string(note.updated_at).c_str(),
                      -1,
                      SQLITE_TRANSIENT);

    sqlite3_bind_text(stmt,
                      5,
                      note.status ? to_string(*note.status).c_str() : nullptr,
                      -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,
                      6,
                      note.due_date ? note::to_string(*note.due_date).c_str()
                                    : nullptr,
                      -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(
        stmt,
        7,
        note.start_date ? note::to_string(*note.start_date).c_str() : nullptr,
        -1,
        SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,
                      8,
                      note.end_date ? note::to_string(*note.end_date).c_str()
                                    : nullptr,
                      -1,
                      SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Insert failed");
    }

    note::Id new_id = static_cast<note::Id>(sqlite3_last_insert_rowid(db_));
    sqlite3_finalize(stmt);
    return new_id;
}

std::optional<note::Note> SqliteRepository::getNote(note::Id id) const
{
    const char *sql = "SELECT * FROM notes WHERE id = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return std::nullopt;

    sqlite3_bind_int(stmt, 1, id);

    note::Note note;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        note = this->extractNote(stmt);
        sqlite3_finalize(stmt);
        return note;
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}

std::vector<note::Note> SqliteRepository::getAllNotes() const
{
    return queryNotesByType(note::NoteType::MEMO /* dummy, will not filter */);
}

std::vector<note::Note> SqliteRepository::getAllMemos() const
{
    return queryNotesByType(note::NoteType::MEMO);
}

std::vector<note::Note> SqliteRepository::getAllTasks() const
{
    return queryNotesByType(note::NoteType::TASK);
}

std::vector<note::Note> SqliteRepository::getAllEvents() const
{
    return queryNotesByType(note::NoteType::EVENT);
}

std::vector<note::Note>
SqliteRepository::queryNotesByType(note::NoteType type) const
{
    std::vector<note::Note> notes;

    std::string sql =
        (type == note::NoteType::MEMO || type == note::NoteType::TASK ||
         type == note::NoteType::EVENT)
        ? "SELECT * FROM notes WHERE type = ?"
        : "SELECT * FROM notes";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return notes;

    if (type == note::NoteType::MEMO || type == note::NoteType::TASK ||
        type == note::NoteType::EVENT)
        sqlite3_bind_text(
            stmt, 1, to_string(type).c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        notes.push_back(extractNote(stmt));
    }

    sqlite3_finalize(stmt);
    return notes;
}

bool SqliteRepository::updateNote(const note::Note &note)
{
    const char *sql = R"(
        UPDATE notes
        SET type = ?, content = ?, created_at = ?, updated_at = ?, status = ?, due_date = ?, start_date = ?, end_date = ?
        WHERE id = ?;
    )";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(
        stmt, 1, note::to_string(note.type).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, note.content.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,
                      3,
                      note::to_string(note.created_at).c_str(),
                      -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,
                      4,
                      note::to_string(note.updated_at).c_str(),
                      -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,
                      5,
                      note.status ? note::to_string(*note.status).c_str()
                                  : nullptr,
                      -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,
                      6,
                      note.due_date ? note::to_string(*note.due_date).c_str()
                                    : nullptr,
                      -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(
        stmt,
        7,
        note.start_date ? note::to_string(*note.start_date).c_str() : nullptr,
        -1,
        SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,
                      8,
                      note.end_date ? note::to_string(*note.end_date).c_str()
                                    : nullptr,
                      -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 9, note.id);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool SqliteRepository::deleteNote(note::Id id)
{
    const char *sql = "DELETE FROM notes WHERE id = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, id);
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

note::Note SqliteRepository::extractNote(sqlite3_stmt *stmt) const
{
    note::Note note;
    note.id = sqlite3_column_int(stmt, 0);
    note.type = static_cast<note::NoteType>(
        std::string(reinterpret_cast<const char *>(
            sqlite3_column_text(stmt, 1))) == "TASK"
            ? note::NoteType::TASK
            : (std::string(reinterpret_cast<const char *>(
                   sqlite3_column_text(stmt, 1))) == "EVENT"
                   ? note::NoteType::EVENT
                   : note::NoteType::MEMO));
    note.content = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    note.created_at = note::parse_time(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));
    note.updated_at = note::parse_time(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));

    if (sqlite3_column_type(stmt, 5) != SQLITE_NULL)
        note.status = static_cast<note::NoteStatus>(
            std::string(reinterpret_cast<const char *>(
                sqlite3_column_text(stmt, 5))) == "DOING"
                ? note::NoteStatus::DOING
                : (std::string(reinterpret_cast<const char *>(
                       sqlite3_column_text(stmt, 5))) == "DONE"
                       ? note::NoteStatus::DONE
                       : note::NoteStatus::TODO));

    if (sqlite3_column_type(stmt, 6) != SQLITE_NULL)
        note.due_date = note::parse_time(
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6)));

    if (sqlite3_column_type(stmt, 7) != SQLITE_NULL)
        note.start_date = note::parse_time(
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7)));

    if (sqlite3_column_type(stmt, 8) != SQLITE_NULL)
        note.end_date = note::parse_time(
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 8)));

    return note;
}

} // namespace banchoo::repository
