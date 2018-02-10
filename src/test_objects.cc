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

serializer_t serializer{};

inmemory_object_store_t<serializer_t> inc_object_store() {
    return inmemory_object_store_t<serializer_t>(incrementing_hash_func{}, serializer_t{});
}

/**
 * Used for tests, equals means their serialization is equal.
 */
bool operator==(const object_t& obj, const object_t& other) {
    return serializer.serialize(obj) == serializer.serialize(other);
}

TEST(serializer_serialize, blob) {
    blob_t blob{"a1"};
    EXPECT_EQ("a1", serializer.serialize(blob));
}

TEST(serializer_serialize, commit) {
    commit_t commit{"a2"};
    EXPECT_EQ("a2", serializer.serialize(commit));
}

TEST(serializer_deserialize, blob) {
    auto blob{serializer.deserialize_blob("b1")};
    ASSERT_TRUE(bool(blob));
    EXPECT_TRUE(blob_t{"b1"} == *blob);
}

TEST(serializer_deserialize, commit) {
    auto commit{serializer.deserialize_commit("b2")};
    ASSERT_TRUE(bool(commit));
    EXPECT_TRUE(commit_t{"b2"} == *commit);
}

TEST(inmemory_object_store, save_returns_hash) {
    auto objects(inc_object_store());
    EXPECT_EQ(0, objects.save(commit_t{"a3"}));
    EXPECT_EQ(1, objects.save(commit_t{"a4"}));
}

TEST(inmemory_object_store, load_unexisting_hash) {
    auto objects(inc_object_store());
    auto object(objects.load_object(0));
    EXPECT_FALSE(bool(object));
}

TEST(inmemory_object_store, load_returns_saved_string) {
    auto objects(inc_object_store());
    commit_t commit{"a5"};
    cit::hash_t hash = objects.save(commit);
    auto loaded(objects.load_object(hash));
    ASSERT_TRUE(bool(loaded));
    EXPECT_EQ(serializer.serialize(commit), *loaded);
}

TEST(inmemory_object_store, load_commit_returns_saved) {
    auto objects(inc_object_store());
    commit_t commit{"a6"};
    cit::hash_t hash = objects.save(commit);
    auto loaded_commit(objects.load_commit(hash));
    ASSERT_TRUE(bool(loaded_commit));
    EXPECT_TRUE(commit == *loaded_commit);
}

TEST(inmemory_object_store, load_blob_returns_saved) {
    auto objects(inc_object_store());
    blob_t blob{"a7"};
    cit::hash_t hash = objects.save(blob);
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

TEST(inmemory_object_store, load_deserializtion_fails) {
    inmemory_object_store_t<failing_deserializtion> objects(incrementing_hash_func{}, failing_deserializtion{});
    commit_t commit{"a8"};
    cit::hash_t hash = objects.save(commit);
    EXPECT_TRUE(bool(objects.load_object(hash)));
    EXPECT_FALSE(bool(objects.load_commit(hash)));
}
