#include  <iostream>

#include "gtest/gtest.h"

#include "objects.h"

using namespace cit;

std::string read_stream(std::istream& in) {
    std::string s;
    in >> s;
    return s;
}

struct incrementing_hash_func {
    /**
     * This obviously isn't a real hash function,
     * but it is enough for our tests.
     */
    hash_t hash = 0;
    hash_t operator()(object_t&) {
        return hash++;
    }
};

inmemory_object_store_t<inmemory_commit_t> inc_object_store() {
    return inmemory_object_store_t<inmemory_commit_t>(incrementing_hash_func{});
}

TEST(blob, returns_content) {
    inmemory_blob_t blob("asdf");
    EXPECT_EQ(blob.content, read_stream(*blob.get_content()));
}

TEST(commit, returns_content) {
    auto commit{std::make_unique<inmemory_commit_t>("asdf")};
    EXPECT_EQ(commit->description, read_stream(*commit->get_content()));
}

TEST(inmemory_object_store, save_returns_hash) {
    auto commit{std::make_unique<inmemory_commit_t>("asdf")};
    auto objects(inc_object_store());
    EXPECT_EQ(0, objects.save(std::move(commit)));
    EXPECT_EQ(1, objects.save(std::move(commit)));
}

TEST(inmemory_object_store, load_returns_saved) {
    auto commit{std::make_unique<inmemory_commit_t>("asdf")};
    auto objects(inc_object_store());
    cit::hash_t id = objects.save(std::move(commit));
    auto loaded_commit(objects.load_commit(id));
    EXPECT_EQ("asdf",read_stream(*loaded_commit.get_content()));
}
