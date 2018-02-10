#include  <iostream>

#include "gtest/gtest.h"

#include "objects.h"

using namespace cit;

struct incrementing_hash_func {
    /**
     * This obviously isn't a real hash function,
     * but it is enough for our tests.
     */
    hash_t hash = 0;
    hash_t operator()(const std::string&) {
        return hash++;
    }
};

inmemory_object_store_t inc_object_store() {
    return inmemory_object_store_t(incrementing_hash_func{});
}

TEST(serialize, blob) {
    blob_t blob{"asdf"};
    EXPECT_EQ("asdf", serialize(blob));
}

TEST(commit, returns_content) {
    commit_t commit{"asdf"};
    EXPECT_EQ("asdf", serialize(commit));
}

TEST(inmemory_object_store, save_returns_hash) {
    auto objects(inc_object_store());
    EXPECT_EQ(0, objects.save(commit_t{"asdf"}));
    EXPECT_EQ(1, objects.save(commit_t{"asdf"}));
}

TEST(inmemory_object_store, load_returns_saved) {
    auto objects(inc_object_store());
    cit::hash_t hash = objects.save(commit_t{"asdf"});
    auto loaded_commit(objects.load_commit(hash));
    ASSERT_TRUE(loaded_commit.has_value());
    EXPECT_EQ("asdf", serialize(*loaded_commit));
}

TEST(inmemory_object_store, load_unexisting_hash) {
    auto objects(inc_object_store());
    auto commit(objects.load_commit(0));
    EXPECT_FALSE(commit.has_value());
}
