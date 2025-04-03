#include <doctest/doctest.h>

#include "repository/inmemory_repository.hpp"

TEST_CASE("InMemoryRepository")
{
    banchoo::repository::InMemoryRepository repo;

    SUBCASE("createNote")
    {
        banchoo::note::Note n;
        n.content = "Hello, World!";
        auto id = repo.createNote(n);
        CHECK(id == 1);
    }

    SUBCASE("getNote")
    {
        banchoo::note::Note n;
        n.content = "Hello, World!";
        auto id = repo.createNote(n);

        auto result = repo.getNote(id);
        REQUIRE(result.has_value());
        CHECK(result->content == "Hello, World!");
    }

    SUBCASE("getAllNotes")
    {
        banchoo::note::Note n1;
        n1.content = "Hello, World!";
        auto id1 = repo.createNote(n1);

        banchoo::note::Note n2;
        n2.content = "Hello, C++!";
        auto id2 = repo.createNote(n2);

        auto all = repo.getAllNotes();

        std::sort(all.begin(), all.end(),
                  [](const banchoo::note::Note &a, const banchoo::note::Note &b) { return a.id < b.id; });

        REQUIRE(all.size() == 2);
        CHECK(all[0].content == "Hello, World!");
        CHECK(all[1].content == "Hello, C++!");
    }

    SUBCASE("updateNote")
    {
        banchoo::note::Note n;
        n.content = "Hello, World!";
        auto id = repo.createNote(n);

        {
            auto result = repo.getNote(id);
            REQUIRE(result.has_value());
            result.value().content = "Hello, C++!";
            CHECK(repo.updateNote(result.value()));
        }

        {
            auto result = repo.getNote(id);
            REQUIRE(result.has_value());
            CHECK(result.value().content == "Hello, C++!");
        }
    }

    SUBCASE("deleteNote")
    {
        banchoo::note::Note n;
        n.content = "Hello, World!";
        auto id = repo.createNote(n);

        CHECK(repo.deleteNote(id));

        auto result = repo.getNote(id);
        CHECK(!result.has_value());
    }
}