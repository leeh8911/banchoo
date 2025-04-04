/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include "repository/inmemory_repository.hpp"
#include "repository/base_repository.hpp"

#include <mutex>
#include <ranges>
#include <unordered_map>
#include <vector>

#include "common/logger.hpp"
#include "note/note.hpp"

namespace banchoo::repository
{

note::Id InMemoryRepository::createNote(const note::Note &note)
{
    std::lock_guard<std::mutex> lock(mutex_);
    notes_[note.id] = note;

    return note.id;
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
    all.reserve(notes_.size());
    for (const auto &[_, n] : notes_)
    {
        all.push_back(n);
    }
    return all;
}

std::vector<note::Note> InMemoryRepository::getAllMemos() const
{
    auto all = getAllNotes();
    auto filtered =
        all | std::views::filter([](const note::Note &note)
                                 { return note.type == note::NoteType::MEMO; });
    std::vector<note::Note> filtered_notes(filtered.begin(), filtered.end());
    return filtered_notes;
}

std::vector<note::Note> InMemoryRepository::getAllTasks() const
{
    auto all = getAllNotes();
    auto filtered =
        all | std::views::filter([](const note::Note &note)
                                 { return note.type == note::NoteType::TASK; });
    std::vector<note::Note> filtered_notes(filtered.begin(), filtered.end());
    return all;
}
std::vector<note::Note> InMemoryRepository::getAllEvents() const
{
    auto all      = getAllNotes();
    auto filtered = all | std::views::filter(
                              [](const note::Note &note)
                              { return note.type == note::NoteType::EVENT; });
    std::vector<note::Note> filtered_notes(filtered.begin(), filtered.end());
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
