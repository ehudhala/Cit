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

inmemory_object_store_t inc_object_store() {
    return inmemory_object_store_t(incrementing_hash_func{});
}

TEST(blob, returns_content) {
    inmemory_blob_t blob("asdf");
    EXPECT_EQ(blob.content, read_stream(*blob.get_content()));
}

TEST(commit, returns_content) {
    commit_t commit("asdf");
    EXPECT_EQ(commit.description, read_stream(*commit.get_content()));
}

TEST(inmemory_object_store, save) {
    commit_t commit("asdf");
    inmemory_object_store_t objects(inc_object_store());
    EXPECT_EQ(0, objects.save(commit));
    EXPECT_EQ(1, objects.save(commit));
}
