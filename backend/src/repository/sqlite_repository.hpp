/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include <sqlite/sqlite3.h>

#include "repository/base_repository.hpp"

namespace banchoo::repository
{

class SqliteRepository : public BaseRepository
{
 public:
    explicit SqliteRepository(const nlohmann::json &config);
    ~SqliteRepository() override;
    note::Id createNote(const note::Note &note) override;

    std::optional<note::Note> getNote(note::Id id) const override;
    std::vector<note::Note> getAllNotes() const override;
    std::vector<note::Note> getAllMemos() const override;
    std::vector<note::Note> getAllTasks() const override;
    std::vector<note::Note> getAllEvents() const override;
    bool updateNote(const note::Note &note) override;
    bool deleteNote(note::Id id) override;

 private:
    sqlite3 *db_;
    void initializeDatabase() const;
    std::vector<note::Note> queryNotesByType(note::NoteType type) const;
    note::Note extractNote(sqlite3_stmt *stmt) const;
};

} // namespace banchoo::repository
