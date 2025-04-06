#include "repository/sqlite_repository.hpp"

#include <nlohmann/json.hpp>

#include "repository/sqlite_handler.hpp"

namespace banchoo::repository
{

SqliteRepository::SqliteRepository(const nlohmann::json &config)
    : sqlite_handler(std::make_unique<SqliteHandler>(config))
{
}

SqliteRepository::~SqliteRepository() = default;

void SqliteRepository::initializeDatabase() const
{
    this->sqlite_handler->initializeDatabase();
}

note::Id SqliteRepository::createNote(const note::Note &note)
{
    return this->sqlite_handler->createNote(note);
}

std::optional<note::Note> SqliteRepository::getNote(note::Id id) const
{
    return this->sqlite_handler->getNote(id);
}

std::vector<note::Note> SqliteRepository::getAllNotes() const
{
    return this->sqlite_handler->queryNotesByType(
        note::NoteType::MEMO); // 전체 가져오기 구현 시 수정 가능
}

std::vector<note::Note> SqliteRepository::getAllMemos() const
{
    return this->sqlite_handler->queryNotesByType(note::NoteType::MEMO);
}

std::vector<note::Note> SqliteRepository::getAllTasks() const
{
    return this->sqlite_handler->queryNotesByType(note::NoteType::TASK);
}

std::vector<note::Note> SqliteRepository::getAllEvents() const
{
    return this->sqlite_handler->queryNotesByType(note::NoteType::EVENT);
}

std::vector<note::Note>
SqliteRepository::queryNotesByType(note::NoteType type) const
{
    return this->sqlite_handler->queryNotesByType(type);
}

bool SqliteRepository::updateNote(const note::Note &note)
{
    return this->sqlite_handler->updateNote(note);
}

bool SqliteRepository::deleteNote(note::Id id)
{
    return this->sqlite_handler->deleteNote(id);
}

} // namespace banchoo::repository
