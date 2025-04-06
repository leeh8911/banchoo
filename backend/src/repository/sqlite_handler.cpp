/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#include "repository/sqlite_handler.hpp"

#include <filesystem>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "note/note.hpp"

namespace banchoo::repository
{

SqliteHandler::SqliteHandler(const nlohmann::json &config) : db_(nullptr)
{
    std::string db_path = config["db_path"].get<std::string>();
    if (db_path.empty())
        throw std::invalid_argument("Database path is empty");

    std::filesystem::path db_file_path(db_path);
    auto dir = db_file_path.parent_path();
    if (!dir.empty() && !std::filesystem::exists(dir))
    {
        std::filesystem::create_directories(dir);
    }

    if (sqlite3_open(db_path.c_str(), &db_) != SQLITE_OK)
        throw std::runtime_error("Failed to open database: " +
                                 std::string(sqlite3_errmsg(db_)));

    initializeDatabase();
}

SqliteHandler::~SqliteHandler()
{
    sqlite3_close(db_);
}

void SqliteHandler::initializeDatabase() const
{
    const char *notes_sql = R"(
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

    const char *tags_sql = R"(
        CREATE TABLE IF NOT EXISTS note_tags (
            note_id INTEGER,
            tag TEXT,
            FOREIGN KEY(note_id) REFERENCES notes(id) ON DELETE CASCADE
        );
    )";

    char *errMsg = nullptr;
    if (sqlite3_exec(db_, notes_sql, nullptr, nullptr, &errMsg) != SQLITE_OK ||
        sqlite3_exec(db_, tags_sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::string error = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error("Failed to create tables: " + error);
    }
}

note::Id SqliteHandler::createNote(const note::Note &note)
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

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Insert failed");
    }

    note::Id new_id = static_cast<note::Id>(sqlite3_last_insert_rowid(db_));
    sqlite3_finalize(stmt);

    for (const auto &tag : note.tags)
    {
        const char *tag_sql =
            "INSERT INTO note_tags (note_id, tag) VALUES (?, ?)";
        sqlite3_stmt *tag_stmt;
        sqlite3_prepare_v2(db_, tag_sql, -1, &tag_stmt, nullptr);
        sqlite3_bind_int(tag_stmt, 1, new_id);
        sqlite3_bind_text(tag_stmt, 2, tag.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(tag_stmt);
        sqlite3_finalize(tag_stmt);
    }

    return new_id;
}

bool SqliteHandler::updateNote(const note::Note &note)
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

    if (success)
    {
        const char *del_sql = "DELETE FROM note_tags WHERE note_id = ?";
        sqlite3_stmt *del_stmt;
        sqlite3_prepare_v2(db_, del_sql, -1, &del_stmt, nullptr);
        sqlite3_bind_int(del_stmt, 1, note.id);
        sqlite3_step(del_stmt);
        sqlite3_finalize(del_stmt);

        for (const auto &tag : note.tags)
        {
            const char *tag_sql =
                "INSERT INTO note_tags (note_id, tag) VALUES (?, ?)";
            sqlite3_stmt *tag_stmt;
            sqlite3_prepare_v2(db_, tag_sql, -1, &tag_stmt, nullptr);
            sqlite3_bind_int(tag_stmt, 1, note.id);
            sqlite3_bind_text(tag_stmt, 2, tag.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_step(tag_stmt);
            sqlite3_finalize(tag_stmt);
        }
    }
    return success;
}

bool SqliteHandler::deleteNote(note::Id id) const
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

std::optional<note::Note> SqliteHandler::getNote(note::Id id) const
{
    const char *sql = "SELECT * FROM notes WHERE id = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return std::nullopt;

    sqlite3_bind_int(stmt, 1, id);
    note::Note note;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        note = extractNote(stmt);
        sqlite3_finalize(stmt);
        return note;
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}

std::vector<note::Note>
SqliteHandler::queryNotesByType(note::NoteType type) const
{
    std::vector<note::Note> notes;
    const char *sql = "SELECT * FROM notes WHERE type = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return notes;

    sqlite3_bind_text(stmt, 1, to_string(type).c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        notes.push_back(extractNote(stmt));
    }

    sqlite3_finalize(stmt);
    return notes;
}

std::vector<note::Note>
SqliteHandler::queryNotesByTag(const std::string &tag) const
{
    std::vector<note::Note> notes;
    const char *sql = R"(
        SELECT n.* FROM notes n
        JOIN note_tags t ON n.id = t.note_id
        WHERE t.tag = ?
    )";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return notes;

    sqlite3_bind_text(stmt, 1, tag.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        notes.push_back(extractNote(stmt));
    }

    sqlite3_finalize(stmt);
    return notes;
}

note::Note SqliteHandler::extractNote(sqlite3_stmt *stmt) const
{
    note::Note note;
    note.id = sqlite3_column_int(stmt, 0);
    std::string type_str =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    if (type_str == "TASK")
        note.type = note::NoteType::TASK;
    else if (type_str == "EVENT")
        note.type = note::NoteType::EVENT;
    else
        note.type = note::NoteType::MEMO;

    note.content = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    note.created_at = note::parse_time(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));
    note.updated_at = note::parse_time(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));

    if (sqlite3_column_type(stmt, 5) != SQLITE_NULL)
    {
        std::string status_str =
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        if (status_str == "DOING")
            note.status = note::NoteStatus::DOING;
        else if (status_str == "DONE")
            note.status = note::NoteStatus::DONE;
        else
            note.status = note::NoteStatus::TODO;
    }

    if (sqlite3_column_type(stmt, 6) != SQLITE_NULL)
        note.due_date = note::parse_time(
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6)));
    if (sqlite3_column_type(stmt, 7) != SQLITE_NULL)
        note.start_date = note::parse_time(
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7)));
    if (sqlite3_column_type(stmt, 8) != SQLITE_NULL)
        note.end_date = note::parse_time(
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 8)));

    const char *tag_sql = "SELECT tag FROM note_tags WHERE note_id = ?";
    sqlite3_stmt *tag_stmt;
    sqlite3_prepare_v2(db_, tag_sql, -1, &tag_stmt, nullptr);
    sqlite3_bind_int(tag_stmt, 1, note.id);

    while (sqlite3_step(tag_stmt) == SQLITE_ROW)
    {
        const char *tag_cstr =
            reinterpret_cast<const char *>(sqlite3_column_text(tag_stmt, 0));
        if (tag_cstr)
            note.tags.push_back(tag_cstr);
    }

    sqlite3_finalize(tag_stmt);
    return note;
}

} // namespace banchoo::repository
