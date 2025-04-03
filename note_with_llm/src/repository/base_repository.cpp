/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include "repository/base_repository.hpp"

#include "common/logger.hpp"
#include "note/note.hpp"

namespace banchoo::repository
{
note::Id BaseRepository::createMemo(const note::Note &note)
{
    note::Note new_note = note;
    new_note.id = this->newId();
    new_note.type = note::NoteType::MEMO;
    new_note.created_at = std::chrono::system_clock::now();
    new_note.updated_at = note.created_at;

    BANCHOO_TRACE("Create memo: id: {}, type: {}, created_at: {}", new_note.id, banchoo::note::to_string(new_note.type),
                  banchoo::note::to_string(new_note.created_at));

    return this->createNote(new_note);
}

note::Id BaseRepository::createTask(const note::Note &note)
{
    BANCHOO_DEBUG("Create task: {}", note.content);

    note::Note new_note = note;
    new_note.id = this->newId();
    new_note.type = note::NoteType::TASK;
    new_note.created_at = std::chrono::system_clock::now();
    new_note.updated_at = note.created_at;

    new_note.status = note::NoteStatus::TODO;

    BANCHOO_TRACE("Create task: id: {}, type: {}, created_at: {}, status: {}", new_note.id,
                  banchoo::note::to_string(new_note.type), banchoo::note::to_string(new_note.created_at),
                  banchoo::note::to_string(new_note.status.value()));

    return this->createNote(new_note);
}

note::Id BaseRepository::createEvent(const note::Note &note)
{
    BANCHOO_DEBUG("Create event: {}", note.content);

    note::Note new_note = note;
    new_note.id = this->newId();
    new_note.type = note::NoteType::EVENT;
    new_note.created_at = std::chrono::system_clock::now();
    new_note.updated_at = note.created_at;

    BANCHOO_TRACE("Create event: id: {}, type: {}, created_at: {}", new_note.id,
                  banchoo::note::to_string(new_note.type), banchoo::note::to_string(new_note.created_at));

    return this->createNote(new_note);
}

note::Id BaseRepository::newId()
{
    return next_id_++;
}

} // namespace banchoo::repository
