#include "gtest/gtest.h"

#include "working_tree.h"

using namespace cit;
using namespace inmemory;

TEST(working_tree_list, returns_names) {
    working_tree_t tree{{{"ab", "bc"}, {"cd", "ef"}}};
    std::vector<name_t> expected{"ab", "cd"};
    EXPECT_EQ(expected, tree.list());
}

TEST(working_tree_read, existing) {
    working_tree_t tree{{{"ab", "bc"}}};
    auto content = tree.read("ab");
    ASSERT_TRUE(bool(content));
    EXPECT_EQ("bc", *content);
}

TEST(working_tree_read, non_existing) {
    working_tree_t tree{{{"ab", "bc"}}};
    ASSERT_FALSE(bool(tree.read("non_existing")));
}

TEST(working_tree_contains, existing) {
    working_tree_t tree{{{"ab", "bc"}}};
    EXPECT_TRUE(tree.contains("ab"));
}

TEST(working_tree_contains, non_existing) {
    working_tree_t tree{{{"ab", "bc"}}};
    EXPECT_FALSE(tree.contains("non_existing"));
}
