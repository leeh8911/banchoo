#pragma once

#include <chrono>
#include <optional>
#include <string>

namespace banchoo::note
{
using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

enum class NoteStatus
{
    TODO,
    DOING,
    DONE
};
enum class NoteType
{
    TASK,
    EVENT,
    MEMO
};

using Id = int;

struct Note
{
    Id id;
    NoteType type;
    NoteStatus status;
    std::string content;
    TimePoint created_at;
    TimePoint updated_at;

    // 일정 정보 (선택적, Task/Event용)
    std::optional<TimePoint> due_date;
    std::optional<TimePoint> start_date;
    std::optional<TimePoint> end_date;
};
} // namespace banchoo::note