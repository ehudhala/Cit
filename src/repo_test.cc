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
