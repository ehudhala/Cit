#include "gtest/gtest.h"

#include "working_tree.h"

using namespace cit;
using namespace inmemory;

TEST(working_tree_list, returns_names) {
    working_tree_t working{{{"ab", "bc"}, {"cd", "ef"}}};
    std::vector<name_t> expected{"ab", "cd"};
    EXPECT_EQ(expected, working.list());
}

TEST(working_tree_read, existing) {
    working_tree_t working{{{"ab", "bc"}}};
    auto content = working.read("ab");
    ASSERT_TRUE(bool(content));
    EXPECT_EQ("bc", *content);
}

TEST(working_tree_read, non_existing) {
    working_tree_t working{{{"ab", "bc"}}};
    ASSERT_FALSE(bool(working.read("non_existing")));
}

TEST(working_tree_contains, existing) {
    working_tree_t working{{{"ab", "bc"}}};
    EXPECT_TRUE(working.contains("ab"));
}

TEST(working_tree_contains, non_existing) {
    working_tree_t working{{{"ab", "bc"}}};
    EXPECT_FALSE(working.contains("non_existing"));
}
