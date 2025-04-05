/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
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
    std::string content;
    TimePoint created_at;
    TimePoint updated_at;

    // Task info
    std::optional<NoteStatus> status;
    std::optional<TimePoint> due_date;

    // Event info
    std::optional<TimePoint> start_date;
    std::optional<TimePoint> end_date;
};

inline std::string to_string(NoteType type)
{
    switch (type)
    {
    case NoteType::TASK:
        return "TASK";
    case NoteType::EVENT:
        return "EVENT";
    case NoteType::MEMO:
        return "MEMO";
    default:
        return "UNKNOWN";
    }
}

inline std::string to_string(NoteStatus status)
{
    switch (status)
    {
    case NoteStatus::TODO:
        return "TODO";
    case NoteStatus::DOING:
        return "DOING";
    case NoteStatus::DONE:
        return "DONE";
    default:
        return "UNKNOWN";
    }
}

inline std::string to_string(const TimePoint &tp)
{
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&time);

    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
    return std::string(buffer);
}
// 예시용 포맷: ISO 8601
inline TimePoint parse_time(const std::string &s)
{
    std::istringstream ss(s);
    std::tm t = {};
    ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"); // ISO8601
    return std::chrono::system_clock::from_time_t(std::mktime(&t));
}
} // namespace banchoo::note
