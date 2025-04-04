/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace banchoo
{

static const std::unordered_map<std::string, spdlog::level::level_enum>
    LOG_LEVELS = {{"trace", spdlog::level::trace},
                  {"debug", spdlog::level::debug},
                  {"info", spdlog::level::info},
                  {"warn", spdlog::level::warn},
                  {"error", spdlog::level::err},
                  {"critical", spdlog::level::critical}};

class Logger
{
public:
    static void init(const std::string &level = "info")
    {
        if (!logger)
        {
            logger = spdlog::stdout_color_mt("banchoo");
            logger->set_pattern(
                "(\%Y-\%m-\%d \%H:\%M:\%S) BANCHOO - [%^%-8l%$] (%s:%#) %v");
            logger->set_level(LOG_LEVELS.at(level));
        }
    }

    static std::shared_ptr<spdlog::logger> &get()
    {
        if (!logger)
        {
            init(); // 기본 info로 초기화
        }
        return logger;
    }

private:
    static inline std::shared_ptr<spdlog::logger> logger = nullptr;
};

// Logging macro shortcuts
#define BANCHOO_TRACE(...)                                                     \
    ::banchoo::Logger::get()->log(                                             \
        spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},               \
        spdlog::level::trace,                                                  \
        __VA_ARGS__)
#define BANCHOO_DEBUG(...)                                                     \
    ::banchoo::Logger::get()->log(                                             \
        spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},               \
        spdlog::level::debug,                                                  \
        __VA_ARGS__)
#define BANCHOO_INFO(...)                                                      \
    ::banchoo::Logger::get()->log(                                             \
        spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},               \
        spdlog::level::info,                                                   \
        __VA_ARGS__)
#define BANCHOO_WARN(...)                                                      \
    ::banchoo::Logger::get()->log(                                             \
        spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},               \
        spdlog::level::warn,                                                   \
        __VA_ARGS__)
#define BANCHOO_ERROR(...)                                                     \
    ::banchoo::Logger::get()->log(                                             \
        spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},               \
        spdlog::level::err,                                                    \
        __VA_ARGS__)
#define BANCHOO_CRITICAL(...)                                                  \
    ::banchoo::Logger::get()->log(                                             \
        spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},               \
        spdlog::level::critical,                                               \
        __VA_ARGS__)

} // namespace banchoo