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

TEST(working_tree, write_creates_new_file) {
    working_tree_t tree{{{}}};
    std::string content = "content";
    tree.write("name", content);
    auto read_content = tree.read("name");
    ASSERT_TRUE(bool(read_content));
    EXPECT_EQ(content, *read_content);
}

TEST(working_tree, write_updates_existing_file) {
    working_tree_t tree{{{"name", "content"}}};
    std::string new_content = "new_content";
    tree.write("name", new_content);
    auto read_content = tree.read("name");
    ASSERT_TRUE(bool(read_content));
    EXPECT_EQ(new_content, *read_content);
}
