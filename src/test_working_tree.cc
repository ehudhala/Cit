#include "gtest/gtest.h"

#include "working_tree.h"

using namespace cit;
using namespace inmemory;

TEST(working_tree_list, returns_names) {
    working_tree_t tree{{{"name", "content"}, {"name2", "content2"}}};
    std::vector<name_t> expected{"name", "name2"};
    EXPECT_EQ(expected, tree.list());
}

TEST(working_tree_read, existing) {
    working_tree_t tree{{{"name", "content"}}};
    auto content = tree.read("name");
    ASSERT_TRUE(bool(content));
    EXPECT_EQ("content", *content);
}

TEST(working_tree_read, non_existing) {
    working_tree_t tree{{{"name", "content"}}};
    EXPECT_FALSE(bool(tree.read("non_existing")));
}

TEST(working_tree_contains, existing) {
    working_tree_t tree{{{"name", "content"}}};
    EXPECT_TRUE(tree.contains("name"));
}

TEST(working_tree_contains, non_existing) {
    working_tree_t tree{{{"name", "content"}}};
    EXPECT_FALSE(tree.contains("non_existing"));
}
