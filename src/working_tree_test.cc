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

TEST(working_tree_write, creates_new_file) {
    working_tree_t tree{{{}}};
    std::string content = "content";
    tree.write("name", content);
    auto read_content = tree.read("name");
    ASSERT_TRUE(bool(read_content));
    EXPECT_EQ(content, *read_content);
}

TEST(working_tree_write, updates_existing_file) {
    working_tree_t tree{{{"name", "content"}}};
    std::string new_content = "new_content";
    tree.write("name", new_content);
    auto read_content = tree.read("name");
    ASSERT_TRUE(bool(read_content));
    EXPECT_EQ(new_content, *read_content);
}

TEST(working_tree_remove, existing_file) {
    working_tree_t tree{{{"name", "content"}}};
    tree.remove("name");
    EXPECT_FALSE(tree.contains("name"));
}

TEST(working_tree_remove, one_of_many_files) {
    working_tree_t tree{{{"name", "content"}, {"name2", "content"}}};
    tree.remove("name");
    EXPECT_TRUE(tree.contains("name2"));
}

TEST(working_tree_remove, non_existing_file) {
    working_tree_t tree{{{"name", "content"}}};
    tree.remove("non_existting_name");
    EXPECT_TRUE(tree.contains("name"));
}

TEST(update_working_tree, updates_existing_files) {
    working_tree_t tree{{{"name", "content"}}};
    tree_content_t new_files{{"name", "new_content"}};
    update_working_tree(tree, new_files);
    auto read_content = tree.read("name");
    ASSERT_TRUE(bool(read_content));
    EXPECT_EQ("new_content", *read_content);
}

TEST(update_working_tree, creates_new_files) {
    working_tree_t tree{{{}}};
    tree_content_t new_files{{"name", "content"}};
    update_working_tree(tree, new_files);
    auto read_content = tree.read("name");
    ASSERT_TRUE(bool(read_content));
    EXPECT_EQ("content", *read_content);
}

TEST(diff_deleted, no_diff) {
    working_tree_t tree{{{"name", "content_doesnt_matter"}}};
    tree_content_t new_files{{"name", "content"}};
    auto diff = diff_deleted(tree.list(), new_files);
    EXPECT_EQ(std::vector<name_t>{}, diff);
}

TEST(diff_deleted, added_files) {
    working_tree_t tree{{{"name", "content"}}};
    tree_content_t new_files{{"name", "content"}, {"new_name", "content"}};
    auto diff = diff_deleted(tree.list(), new_files);
    EXPECT_EQ(std::vector<name_t>{}, diff);
}

TEST(diff_deleted, deleted_files) {
    working_tree_t tree{{{"name", "content"}}};
    tree_content_t new_files{{}};
    auto diff = diff_deleted(tree.list(), new_files);
    EXPECT_EQ(std::vector<name_t>{"name"}, diff);
}

TEST(diff_deleted, deleted_part_of_files) {
    working_tree_t tree{{{"name", "content"}, {"name2", "content"}}};
    tree_content_t new_files{{{"name2", "content"}}};
    auto diff = diff_deleted(tree.list(), new_files);
    EXPECT_EQ(std::vector<name_t>{"name"}, diff);
}

TEST(diff_deleted, deleted_files_and_new_files) {
    working_tree_t tree{{{"name", "content"}}};
    tree_content_t new_files{{"new_name", "content"}};
    auto diff = diff_deleted(tree.list(), new_files);
    EXPECT_EQ(std::vector<name_t>{"name"}, diff);
}
