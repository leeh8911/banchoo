/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <sqlite3.h>

#include <nlohmann/json.hpp>

#include "note/note.hpp"

namespace banchoo::repository
{
class SqliteHandler
{
 public:
    explicit SqliteHandler(const nlohmann::json &config);
    ~SqliteHandler();

    void initializeDatabase() const;

    note::Id createNote(const note::Note &note);

    bool updateNote(const note::Note &note);
    bool deleteNote(note::Id id) const;

    std::optional<note::Note> getNote(note::Id id) const;
    std::vector<note::Note> queryNotesByType(note::NoteType type) const;
    std::vector<note::Note> queryNotesByTag(const std::string &tag) const;

 private:
    note::Note extractNote(sqlite3_stmt *stmt) const;

    sqlite3 *db_;
};
} // namespace banchoo::repository
