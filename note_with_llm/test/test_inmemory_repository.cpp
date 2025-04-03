#include <doctest/doctest.h>

#include "repository/inmemory_repository.hpp"

TEST_CASE("InMemoryRepository")
{
    banchoo::repository::InMemoryRepository repo;

    SUBCASE("createNote")
    {
        banchoo::note::Note n{.content = "Hello, World!"};
        auto id = repo.createNote(n);
        CHECK(id == 1);
    }

    SUBCASE("getNote")
    {
        banchoo::note::Note n{.content = "Hello, World!"};
        auto id = repo.createNote(n);

        auto result = repo.getNote(id);
        REQUIRE(result); // optional이 값 있는지 확인
        CHECK(result->content == "Hello, World!");
    }

    SUBCASE("getAllNotes")
    {
        banchoo::note::Note n1{.content = "Hello, World!"};
        banchoo::note::Note n2{.content = "Hello, C++!"};
        repo.createNote(n1);
        repo.createNote(n2);

        auto all = repo.getAllNotes();
        REQUIRE(all.size() == 2);

        std::sort(all.begin(), all.end(), [](auto &a, auto &b) { return a.id < b.id; });
        CHECK(all[0].content == "Hello, World!");
        CHECK(all[1].content == "Hello, C++!");
    }

    SUBCASE("updateNote")
    {
        banchoo::note::Note n{.content = "Hello, World!"};
        auto id = repo.createNote(n);

        auto original = repo.getNote(id);
        REQUIRE(original);
        original->content = "Hello, C++!";
        CHECK(repo.updateNote(*original));

        auto updated = repo.getNote(id);
        REQUIRE(updated);
        CHECK(updated->content == "Hello, C++!");
    }

    SUBCASE("deleteNote")
    {
        banchoo::note::Note n{.content = "Hello, World!"};
        auto id = repo.createNote(n);

        CHECK(repo.deleteNote(id));

        auto result = repo.getNote(id);
        CHECK_FALSE(result); // 값이 없어야 함
    }
}