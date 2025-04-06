/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#include <doctest/doctest.h>

#include <nlohmann/json.hpp>

#include "common/logger.hpp"
#include "repository/inmemory_repository.hpp"

TEST_CASE("InMemoryRepository")
{
    banchoo::Logger::init("trace"); // 로거 초기화

    banchoo::repository::InMemoryRepository repo(nlohmann::json{});

    SUBCASE("createNote")
    {
        banchoo::note::Note n{.content = "Hello, World!"};
        auto id = repo.createMemo(n);
        CHECK_EQ(id, 1);
    }

    SUBCASE("getNote")
    {
        banchoo::note::Note n{.content = "Hello, World!"};
        auto id = repo.createMemo(n);

        auto result = repo.getNote(id);
        REQUIRE(result); // optional이 값 있는지 확인
        CHECK_EQ(result->content, "Hello, World!");
    }

    SUBCASE("getAllNotes")
    {
        banchoo::note::Note n1{.content = "Hello, World!"};
        banchoo::note::Note n2{.content = "Hello, C++!"};
        repo.createMemo(n1);
        repo.createMemo(n2);

        auto all = repo.getAllNotes();
        REQUIRE(all.size() == 2);

        std::sort(all.begin(),
                  all.end(),
                  [](auto &a, auto &b) { return a.id < b.id; });
        CHECK_EQ(all[0].content, "Hello, World!");
        CHECK_EQ(all[1].content, "Hello, C++!");
    }

    SUBCASE("updateNote")
    {
        banchoo::note::Note n{.content = "Hello, World!"};
        auto id = repo.createMemo(n);

        auto original = repo.getNote(id);
        REQUIRE(original);
        original->content = "Hello, C++!";
        CHECK(repo.updateNote(*original));

        auto updated = repo.getNote(id);
        REQUIRE(updated);
        CHECK_EQ(updated->content, "Hello, C++!");
    }

    SUBCASE("deleteNote")
    {
        banchoo::note::Note n{.content = "Hello, World!"};
        auto id = repo.createMemo(n);

        CHECK(repo.deleteNote(id));

        auto result = repo.getNote(id);
        CHECK_FALSE(result); // 값이 없어야 함
    }
}
