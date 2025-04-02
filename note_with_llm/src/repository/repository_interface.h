/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#pragma once

#include <optional>
#include <vector>

#include "note/note.h"

namespace banchoo::repository
{
class IRepository
{
  public:
    virtual ~IRepository() = default;

    virtual note::Id createNote(const note::Note &note) = 0;
    virtual std::optional<note::Note> getNote(note::Id id) const = 0;
    virtual std::vector<note::Note> getAllNotes() const = 0;
    virtual bool updateNote(const note::Note &note) = 0;
    virtual bool deleteNote(note::Id id) = 0;
};
} // namespace banchoo::repository