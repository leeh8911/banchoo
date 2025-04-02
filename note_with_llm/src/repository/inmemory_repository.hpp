/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#pragma once

#include <mutex>
#include <unordered_map>

#include "repository/repository_interface.hpp"

namespace banchoo::repository
{

class InMemoryRepository : public IRepository
{
  public:
    note::Id createNote(const note::Note &note) override;
    std::optional<note::Note> getNote(note::Id id) const override;
    std::vector<note::Note> getAllNotes() const override;
    bool updateNote(const note::Note &note) override;
    bool deleteNote(note::Id id) override;

  private:
    mutable std::mutex mutex_;
    std::unordered_map<note::Id, note::Note> notes_;
    note::Id next_id_ = 1;
};

} // namespace banchoo::repository
