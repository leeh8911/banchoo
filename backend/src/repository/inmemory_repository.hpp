/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#pragma once

#include <mutex>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include "repository/base_repository.hpp"

namespace banchoo::repository
{

class InMemoryRepository : public BaseRepository
{
 public:
    explicit InMemoryRepository(const nlohmann::json &config);

    note::Id createNote(const note::Note &note) override;

    std::optional<note::Note> getNote(note::Id id) const override;
    std::vector<note::Note> getAllNotes() const override;
    std::vector<note::Note> getAllMemos() const override;
    std::vector<note::Note> getAllTasks() const override;
    std::vector<note::Note> getAllEvents() const override;
    bool updateNote(const note::Note &note) override;
    bool deleteNote(note::Id id) override;

 private:
    mutable std::mutex mutex_;
    std::unordered_map<note::Id, note::Note> notes_;
};

} // namespace banchoo::repository
