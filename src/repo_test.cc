#include "gtest/gtest.h"

#include "repo.h"
#include "test_utils.h"

TEST(repo_add, returns_none_on_non_existing_name) {
    auto r{inc_repo(working_tree_t{{{"name", "content"}}})};
    auto hash = r.add("non_existing_name");
    EXPECT_FALSE(hash);
}

TEST(repo_add, adds_blob_to_index) {
    auto r{inc_repo(working_tree_t{{{"name", "content"}}})};
    auto hash = r.add("name");
    ASSERT_TRUE(bool(hash));
    auto index_file = find_file(r.store.index.files, "name");
    ASSERT_TRUE(bool(index_file));
    EXPECT_EQ(hash, index_file->hash);
}

TEST(repo_add, adds_blob_to_store) {
    auto r{inc_repo(working_tree_t{{{"name", "content"}}})};
    auto hash = r.add("name");
    ASSERT_TRUE(bool(hash));
    EXPECT_TRUE(blob_t{"content"} == *(r.store.get_objects().load<blob_t>(*hash)));
}

TEST(repo_commit, stores_tree) {
    auto r{added_inc_repo(working_tree_t{{{"name", "content"}}})};
    r.commit("message");
    auto loaded_tree = r.store.get_objects().load<tree_t>(1); // incrementing hash :)
    ASSERT_TRUE(bool(loaded_tree));
    tree_t expected{{{"name", 0}}};
    ASSERT_EQ(expected, *loaded_tree);

}

TEST(repo_commit, returns_commit_hash) {
    auto r{inc_repo(working_tree_t{{}})};
    auto commit_hash = r.commit("message");
    ASSERT_TRUE(bool(commit_hash));
    ASSERT_EQ(1, *commit_hash); // incrementing hash.
}

TEST(repo_commit, stores_commit_with_no_parent_hash) {
    auto r{added_inc_repo(working_tree_t{{{"name", "content"}}})};
    auto commit_hash = r.commit("message");
    ASSERT_TRUE(bool(commit_hash));
    auto loaded_commit = r.store.get_objects().load<commit_t>(*commit_hash);
    ASSERT_TRUE(bool(loaded_commit));
    commit_t expected{"message", 1}; // incrementing hash :)
    ASSERT_EQ(expected, *loaded_commit);
}

TEST(repo_commit, creates_commit_with_correct_parent_hash) {
}

TEST(repo_commit, updates_head_to_new_commit) {
}

TEST(repo_commit, updates_refs_to_new_commit) {
}

TEST(repo_commit, returns_none_when_there_are_no_changes) {
}
