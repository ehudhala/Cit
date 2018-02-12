#include "gtest/gtest.h"

#include "objects.h"
#include "store.h"

#include "test_utils.h"

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

inmemory::object_store_t<serializer_t> inc_object_store() {
    return inmemory::object_store_t<serializer_t>(incrementing_hash_func{}, serializer_t{});
}

TEST(inmemory_object_store_store, returns_hash) {
    auto objects(inc_object_store());
    EXPECT_EQ(0, objects.store(commit_t{"a3"}));
    EXPECT_EQ(1, objects.store(commit_t{"a4"}));
}

TEST(inmemory_object_store_load, unexisting_hash) {
    auto objects(inc_object_store());
    auto object(objects.load_object(0));
    EXPECT_FALSE(bool(object));
}

TEST(inmemory_object_store_load, returns_stored_string) {
    auto objects(inc_object_store());
    commit_t commit{"a5"};
    cit::hash_t hash = objects.store(commit);
    auto loaded(objects.load_object(hash));
    ASSERT_TRUE(bool(loaded));
    EXPECT_EQ(serializer.serialize(commit), *loaded);
}

TEST(inmemory_object_store_load, commit_returns_stored) {
    auto objects(inc_object_store());
    commit_t commit{"a6"};
    cit::hash_t hash = objects.store(commit);
    auto loaded_commit(objects.load_commit(hash));
    ASSERT_TRUE(bool(loaded_commit));
    EXPECT_TRUE(commit == *loaded_commit);
}

TEST(inmemory_object_store_load, blob_returns_stored) {
    auto objects(inc_object_store());
    blob_t blob{"a7"};
    cit::hash_t hash = objects.store(blob);
    auto loaded(objects.load_commit(hash));
    ASSERT_TRUE(bool(loaded));
    EXPECT_TRUE(blob == *loaded);
}

struct failing_deserializtion {
    std::string serialize(const object_t&) const {return "";}
    boost::optional<commit_t> deserialize_commit(const std::string&) const {
        return boost::none;
    }
};

TEST(inmemory_object_store_load, deserializtion_fails) {
    inmemory::object_store_t<failing_deserializtion> objects(incrementing_hash_func{}, failing_deserializtion{});
    commit_t commit{"a8"};
    cit::hash_t hash = objects.store(commit);
    EXPECT_TRUE(bool(objects.load_object(hash)));
    EXPECT_FALSE(bool(objects.load_commit(hash)));
}
