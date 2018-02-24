#include "gtest/gtest.h"

#include "repo.h"
#include "test_utils.h"

TEST(repo_add, returns_none_on_non_existing_name) {
    auto r{inc_repo(working_tree_t{{{"ab", "bc"}}})};
}

TEST(repo_add, adds_blob_to_index) {
}
