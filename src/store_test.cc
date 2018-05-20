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

TEST(ref_store, load_non_existent) {
    inmemory::ref_store_t ref_store;
    EXPECT_FALSE(bool(ref_store.load("non_existent")));
}

TEST(ref_store, update_and_load_hash) {
    inmemory::ref_store_t ref_store;
    ref_store.update("hash", 123);
    auto ref{ref_store.load("hash")};
    ASSERT_TRUE(bool(ref));
    EXPECT_EQ(123, boost::get<hash_t>(*ref));
}

TEST(ref_store, update_and_load_recursive_ref_name) {
    inmemory::ref_store_t ref_store;
    ref_store.update("hash", 123);
    ref_store.update("name", "hash");
    auto name_ref{ref_store.load("name")};
    ASSERT_TRUE(bool(name_ref));
    EXPECT_EQ("hash", boost::get<ref_name_t>(*name_ref));
}

TEST(get_ref_hash, hash_ref) {
    inmemory::ref_store_t ref_store;
    auto hash = get_ref_hash(ref_store, 123);
    ASSERT_TRUE(bool(hash));
    EXPECT_EQ(123, *hash);
}

TEST(get_ref_hash, name_not_pointing_to_hash) {
    inmemory::ref_store_t ref_store;
    auto hash = get_ref_hash(ref_store, "non_existent_name");
    ASSERT_FALSE(bool(hash));
}

TEST(get_ref_hash, name_pointing_to_hash) {
    inmemory::ref_store_t ref_store;
    ref_store.update("hash", 123);
    auto hash = get_ref_hash(ref_store, "hash");
    ASSERT_TRUE(bool(hash));
    EXPECT_EQ(123, *hash);
}

TEST(get_ref_hash, recursive_name_pointing_to_hash) {
    inmemory::ref_store_t ref_store;
    ref_store.update("hash", 123);
    ref_store.update("name", "hash");
    auto hash = get_ref_hash(ref_store, "name");
    ASSERT_TRUE(bool(hash));
    EXPECT_EQ(123, *hash);
}

TEST(update_ref_deep_hash, name_points_to_hash) {
    inmemory::ref_store_t ref_store;
    ref_store.update("hash", 123);
    update_ref_deep_hash(ref_store, "hash", 456);
    auto loaded_hash{ref_store.load("hash")};
    ASSERT_TRUE(bool(loaded_hash));
    EXPECT_EQ(456, boost::get<hash_t>(*loaded_hash));
}

TEST(update_ref_deep_hash, name_points_to_hash_recursively) {
    inmemory::ref_store_t ref_store;
    ref_store.update("hash", 123);
    ref_store.update("name", "hash");
    update_ref_deep_hash(ref_store, "name", 456);
    auto loaded_hash{ref_store.load("hash")};
    ASSERT_TRUE(bool(loaded_hash));
    EXPECT_EQ(456, boost::get<hash_t>(*loaded_hash));
}

TEST(update_ref_deep_hash, name_doesnt_point_to_hash) {
    inmemory::ref_store_t ref_store;
    ref_store.update("name", "hash");
    update_ref_deep_hash(ref_store, "name", 456);
    auto loaded_hash{ref_store.load("hash")};
    ASSERT_TRUE(bool(loaded_hash));
    EXPECT_EQ(456, boost::get<hash_t>(*loaded_hash));
}

TEST(update_ref_hash, given_hash) {
    inmemory::ref_store_t ref_store;
    auto new_hash = update_ref_hash(ref_store, 123, 456);
    EXPECT_EQ(456, boost::get<hash_t>(new_hash));
}

TEST(update_ref_hash, given_name_deep_updates_hash) {
    inmemory::ref_store_t ref_store;
    ref_store.update("hash", 123);
    ref_store.update("name", "hash");
    update_ref_hash(ref_store, "name", 456);
    auto loaded_hash{ref_store.load("hash")};
    ASSERT_TRUE(bool(loaded_hash));
    EXPECT_EQ(456, boost::get<hash_t>(*loaded_hash));
}

TEST(update_ref_hash, given_name_returns_name) {
    inmemory::ref_store_t ref_store;
    auto new_ref = update_ref_hash(ref_store, "name", 456);
    EXPECT_EQ("name", boost::get<ref_name_t>(new_ref));
}

TEST(store_get_update_head_hash, get_no_head_first_commit) {
    store test_store{inc_store()};
    ASSERT_FALSE(bool(test_store.get_head_hash()));
}

TEST(store_get_update_head_hash, update_no_head_first_commit) {
    store test_store{inc_store()};
    test_store.update_head_hash(1);
    auto head_hash = test_store.get_head_hash();
    ASSERT_TRUE(bool(head_hash));
    EXPECT_EQ(1, *head_hash);
}

TEST(store_update_head_hash, head_exists) {
    store test_store{inc_store()};
    test_store.head = "name";
    test_store.update_head_hash(2);
    // Expect ref isn't changed.
    ASSERT_TRUE(bool(test_store.head));
    EXPECT_EQ("name", boost::get<ref_name_t>(*test_store.head));
    // Expect hash changed.
    auto head_hash = test_store.get_head_hash();
    ASSERT_TRUE(bool(head_hash));
    EXPECT_EQ(2, *head_hash);
}
