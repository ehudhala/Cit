#include "gtest/gtest.h"

#include "objects.h"
#include "store.h"

#include "test_utils.h"

using namespace cit;

TEST(inmemory_object_store_store, returns_hash) {
    auto objects(inc_object_store());
    EXPECT_EQ(0, objects.store(commit_t{"a3"}));
    EXPECT_EQ(1, objects.store(commit_t{"a4"}));
}

TEST(inmemory_object_store_load, unexisting_hash) {
    auto objects(inc_object_store());
    auto object(objects.raw_load(0));
    EXPECT_FALSE(bool(object));
}

TEST(inmemory_object_store_load, returns_stored_string) {
    auto objects(inc_object_store());
    commit_t commit{"a5"};
    cit::hash_t hash = objects.store(commit);
    auto loaded(objects.raw_load(hash));
    ASSERT_TRUE(bool(loaded));
    EXPECT_EQ(serializer_t::serialize(commit), *loaded);
}

TEST(inmemory_object_store_load, commit_returns_stored) {
    auto objects(inc_object_store());
    commit_t commit{"a6"};
    cit::hash_t hash = objects.store(commit);
    auto loaded_commit(objects.load<commit_t>(hash));
    ASSERT_TRUE(bool(loaded_commit));
    EXPECT_TRUE(commit == *loaded_commit);
}

TEST(inmemory_object_store_load, blob_returns_stored) {
    auto objects(inc_object_store());
    blob_t blob{"a7"};
    cit::hash_t hash = objects.store(blob);
    auto loaded(objects.load<commit_t>(hash));
    ASSERT_TRUE(bool(loaded));
    EXPECT_TRUE(blob == *loaded);
}

// TODO: add tests to the load method (fail serialization, success on serialization)

struct failing_deserializtion {
    static std::string serialize(const object_t&) {return "";}
    template <class Object>
    static boost::optional<Object> deserialize(const std::string&) {
        return boost::none;
    }
};

TEST(inmemory_object_store_load, deserializtion_fails) {
    inmemory::object_store_t<failing_deserializtion> objects(incrementing_hash_func{});
    commit_t commit{"a8"};
    cit::hash_t hash = objects.store(commit);
    EXPECT_TRUE(bool(objects.raw_load(hash)));
    EXPECT_FALSE(bool(objects.load<commit_t>(hash)));
}

TEST(index_add, returns_hash) {
    index_t index{inc_index()};
    blob_t blob{"asdf"};
    auto hash = index.add("file", blob);
    ASSERT_EQ(0, hash);
}

TEST(index_add, serializes_to_object_store) {
    index_t index{inc_index()};
    blob_t blob{"asdf"};
    auto hash = index.add("file", blob);
    auto loaded(index.objects.load<blob_t>(hash));
    ASSERT_TRUE(bool(loaded));
    EXPECT_TRUE(blob == *loaded);
}

TEST(index_add, adds_to_blob_names) {
    index_t index{inc_index()};
    blob_t blob{"asdf"};
    auto hash = index.add("file", blob);
    EXPECT_EQ(hash, index.blob_names["file"]);
}
