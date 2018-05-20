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
    EXPECT_EQ(hash, index_file->blob_hash);
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
    EXPECT_EQ(expected, *loaded_tree);

}

TEST(repo_commit, returns_commit_hash) {
    auto r{inc_repo(working_tree_t{{}})};
    auto commit_hash = r.commit("message");
    EXPECT_EQ(1, commit_hash); // incrementing hash.
}

TEST(repo_commit, stores_commit_with_no_parent_hash) {
    auto r{added_inc_repo(working_tree_t{{{"name", "content"}}})};
    auto commit_hash = r.commit("message");
    auto loaded_commit = r.store.get_objects().load<commit_t>(commit_hash);
    ASSERT_TRUE(bool(loaded_commit));
    commit_t expected{"message", 1}; // incrementing hash :)
    EXPECT_EQ(expected, *loaded_commit);
}

TEST(repo_commit, creates_commit_with_correct_parent_hash) {
    auto r{inc_repo(working_tree_t{{}})};
    r.store.update_head_hash(123);
    auto commit_hash = r.commit("message");
    auto loaded_commit = r.store.get_objects().load<commit_t>(commit_hash);
    ASSERT_TRUE(bool(loaded_commit));
    commit_t expected("message", 123, 0);
    EXPECT_EQ(expected, *loaded_commit);
}

TEST(repo_commit, updates_head_to_new_commit) {
    auto r{inc_repo(working_tree_t{{}})};
    auto commit_hash = r.commit("message");
    auto head_hash{r.store.get_head_hash()};
    ASSERT_TRUE(bool(head_hash));
    EXPECT_EQ(commit_hash, *head_hash);
}

TEST(repo_commit, updates_refs_to_new_commit) {
    // TODO: implement when we support refs.
}

TEST(repo_checkout, returns_false_when_no_commit_found) {
    auto r{inc_repo(working_tree_t{{}})};
    EXPECT_FALSE(r.checkout(234));
}

TEST(repo_checkout, returns_false_when_hash_isnt_commit) {
    auto r{inc_repo(working_tree_t{{{"name", "content"}}})};
    auto hash = r.add("name");
    EXPECT_FALSE(r.checkout(*hash));
}

TEST(repo_checkout, updates_index_files) {
    std::string content{"content"}, name{"name"};
    auto r{inc_repo(working_tree_t{{{name, content}}})};
    auto blob_hash = r.add(name);
    auto commit_hash = r.commit("message");
    r.working_tree.write(name, "new_content");
    r.add(name);
    r.commit("new_commit");
    r.checkout(commit_hash);
    ASSERT_TRUE(bool(blob_hash));
    std::vector<file_t> expected_files{{name, *blob_hash}};
    EXPECT_EQ(expected_files, r.store.index.files);
}

TEST(repo_checkout, updates_working_tree) {
    std::string content{"content"}, name{"name"}, 
        deleted_on_checkout{"deleted"};
    auto r{inc_repo(working_tree_t{{{name, content}}})};
    auto blob_hash = r.add(name);
    auto commit_hash = r.commit("message");
    r.working_tree.write(name, "new_content");
    r.working_tree.write(deleted_on_checkout, content);
    r.add(name);
    r.add(deleted_on_checkout);
    r.commit("new_commit");
    r.checkout(commit_hash);

    auto working_tree_content = *r.working_tree.read(name);
    EXPECT_EQ(content, working_tree_content);
    EXPECT_FALSE(r.working_tree.contains(deleted_on_checkout));
}

TEST(repo_checkout, updates_head) {
    auto r{inc_repo(working_tree_t{{}})};
    auto commit_hash = r.commit("message");
    r.commit("new_commit");
    r.checkout(commit_hash);
    auto head_hash{r.store.get_head_hash()};
    ASSERT_TRUE(bool(head_hash));
    EXPECT_EQ(commit_hash, *head_hash);
}
