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
    EXPECT_TRUE(blob_t{"content"} == *(r.store.get_objects().load_blob(*hash)));
    auto it = r.store.index.blob_names.find("name");
    ASSERT_TRUE(it != r.store.index.blob_names.end());
    EXPECT_EQ(hash, it->second);
}
