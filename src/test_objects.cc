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
    hash_t operator()(std::istream&) {
        return hash++;
    }
};

inmemory_object_store_t inc_object_store() {
    return inmemory_object_store_t(incrementing_hash_func{});
}

TEST(blob, returns_content) {
    blob_t blob(std::make_shared<std::stringstream>("asdf"));
    EXPECT_EQ("asdf", read_stream(*blob.get_content()));
}

TEST(commit, returns_content) {
    auto commit{std::make_unique<commit_t>("asdf")};
    EXPECT_EQ(commit->description, read_stream(*commit->get_content()));
}

TEST(inmemory_object_store, save_returns_hash) {
    auto objects(inc_object_store());
    EXPECT_EQ(0, objects.save(std::make_unique<commit_t>("asdf")));
    EXPECT_EQ(1, objects.save(std::make_unique<commit_t>("asdf")));
}

TEST(inmemory_object_store, load_returns_saved) {
    auto commit{std::make_unique<commit_t>("asdf")};
    auto objects(inc_object_store());
    cit::hash_t hash = objects.save(std::move(commit));
    auto loaded_commit(objects.load_commit(hash));
    ASSERT_TRUE(loaded_commit.has_value());
    EXPECT_EQ("asdf", read_stream(*(*loaded_commit).get_content()));
}

TEST(inmemory_object_store, load_unexisting_hash) {
    auto objects(inc_object_store());
    auto commit(objects.load_commit(0));
    EXPECT_FALSE(commit.has_value());
}
