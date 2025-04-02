/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#pragma once

#include "repository/inmemory_repository.h"
#include "repository/repository_interface.h"

#include <mutex>
#include <unordered_map>

#include "note/note.h"

namespace banchoo::repository
{

note::Id InMemoryRepository::createNote(const note::Note &note)
{
    std::lock_guard<std::mutex> lock(mutex_);
    note::Note new_note = note;
    new_note.id = next_id_++;
    notes_[new_note.id] = new_note;
    return new_note.id;
}

std::optional<note::Note> InMemoryRepository::getNote(note::Id id) const
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = notes_.find(id);
    if (it != notes_.end())
    {
        return it->second;
    }
    return std::nullopt;
}

std::vector<note::Note> InMemoryRepository::getAllNotes() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<note::Note> all;
    for (const auto &[_, n] : notes_)
    {
        all.push_back(n);
    }
    return all;
}

bool InMemoryRepository::updateNote(const note::Note &note)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = notes_.find(note.id);
    if (it != notes_.end())
    {
        notes_[note.id] = note;
        return true;
    }
    return false;
}

bool InMemoryRepository::deleteNote(note::Id id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    return notes_.erase(id) > 0;
}

} // namespace banchoo::repository
