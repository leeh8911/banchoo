/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#pragma once

#include <optional>
#include <vector>

#include "note/note.hpp"

namespace banchoo::repository
{
class IRepository
{
  public:
    virtual ~IRepository() = default;

    virtual note::Id createNote(const note::Note &note) = 0;

    note::Id createMemo(const note::Note &note);
    note::Id createTask(const note::Note &note);
    note::Id createEvent(const note::Note &note);

    virtual std::optional<note::Note> getNote(note::Id id) const = 0;
    virtual std::vector<note::Note> getAllNotes() const = 0;
    virtual std::vector<note::Note> getAllMemos() const = 0;
    virtual std::vector<note::Note> getAllTasks() const = 0;
    virtual std::vector<note::Note> getAllEvents() const = 0;
    virtual bool updateNote(const note::Note &note) = 0;
    virtual bool deleteNote(note::Id id) = 0;

  protected:
    note::Id newId();

  private:
    note::Id next_id_ = 1;
};
} // namespace banchoo::repository