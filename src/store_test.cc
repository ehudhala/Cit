#include "gtest/gtest.h"

#include "objects.h"
#include "store.h"

#include "test_utils.h"
#include "utils.h"

using namespace cit;

TEST(inmemory_object_store_store, returns_hash) {
    auto objects(inc_object_store());
    EXPECT_EQ(0, objects.store(blob_t{"a3"}));
    EXPECT_EQ(1, objects.store(blob_t{"a4"}));
}

TEST(inmemory_object_store_load, unexisting_hash) {
    auto objects(inc_object_store());
    auto object(objects.raw_load(0));
    EXPECT_FALSE(bool(object));
}

TEST(inmemory_object_store_load, returns_stored_string) {
    auto objects(inc_object_store());
    blob_t blob{"a5"};
    cit::hash_t hash = objects.store(blob);
    auto loaded(objects.raw_load(hash));
    ASSERT_TRUE(bool(loaded));
    EXPECT_EQ(serializer::serialize(blob), *loaded);
}

TEST(inmemory_object_store_load, commit_returns_stored) {
    auto objects(inc_object_store());
    commit_t commit{"a6", 123};
    cit::hash_t hash = objects.store(commit);
    auto loaded_commit(objects.load<commit_t>(hash));
    ASSERT_TRUE(bool(loaded_commit));
    EXPECT_TRUE(commit == *loaded_commit);
}

TEST(inmemory_object_store_load, blob_returns_stored) {
    auto objects(inc_object_store());
    blob_t blob{"a7"};
    cit::hash_t hash = objects.store(blob);
    auto loaded(objects.load<blob_t>(hash));
    ASSERT_TRUE(bool(loaded));
    EXPECT_TRUE(blob == *loaded);
}

TEST(inmemory_object_store_load, deserializtion_fails) {
    inmemory::object_store_t<failing_deserializtion> objects(incrementing_hash_func{});
    blob_t blob{"a8"};
    cit::hash_t hash = objects.store(blob);
    EXPECT_TRUE(bool(objects.raw_load(hash)));
    EXPECT_FALSE(bool(objects.load<blob_t>(hash)));
}

TEST(load_tree, returns_commit_tree) {
    auto objects(inc_object_store());
    tree_t tree{{}};
    auto tree_hash = objects.store(tree);
    auto commit_hash = objects.store(commit_t{"", tree_hash});
    auto loaded_tree = load_tree(objects, commit_hash);
    ASSERT_TRUE(bool(loaded_tree));
    EXPECT_EQ(tree_t{}, *loaded_tree);
}

TEST(load_tree_content, loads_tree_content) {
    auto objects(inc_object_store());
    auto blob_hash = objects.store(blob_t{"content"});
    tree_t tree{{{"name", blob_hash}}};
    auto tree_content = load_tree_content(objects, tree);
    ASSERT_TRUE(bool(tree_content));
    tree_content_t expected_content{{"name", "content"}};
    EXPECT_EQ(expected_content, *tree_content);
}

TEST(load_tree_content, returns_none_when_blob_doesnt_exist) {
    auto objects(inc_object_store());
    tree_t tree{{{"name", 123}}};
    auto tree_content = load_tree_content(objects, tree);
    ASSERT_FALSE(bool(tree_content));
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

TEST(index_add, adds_to_files) {
    index_t index{inc_index()};
    blob_t blob{"asdf"};
    auto hash = index.add("file", blob);
    auto index_file = find_file(index.files, "file");
    ASSERT_TRUE(bool(index_file));
    file_t expected{"file", hash};
    EXPECT_EQ(hash, index_file->blob_hash);
}

TEST(index_update, updates_the_index) {
    index_t index{inc_index()};
    blob_t first_blob{"content"};
    index.add("name", first_blob);
    file_t file{"updated", 123};
    std::vector<file_t> files{file};
    index.update(files);
    EXPECT_EQ(files, index.files);
}
