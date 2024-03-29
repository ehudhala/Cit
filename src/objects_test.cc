#include "gtest/gtest.h"

#include "objects.h"

#include "test_utils.h"

using namespace cit;

TEST(serializer, blob_deserialized_equal_to_serialized) {
    blob_t blob{"content"};
    auto serialized{serializer::serialize(blob)};
    auto deserialized{serializer::deserialize<blob_t>(serialized)};
    ASSERT_TRUE(bool(deserialized));
    EXPECT_TRUE(blob_t{"content"} == *deserialized);
}

TEST(serializer, commit_no_parent_deserialized_equal_to_serialized) {
    commit_t commit{"message", 123};
    auto serialized{serializer::serialize(commit)};
    auto deserialized{serializer::deserialize<commit_t>(serialized)};
    ASSERT_TRUE(bool(deserialized));
    EXPECT_TRUE(commit == *deserialized);
}

TEST(serializer, full_commit_deserialized_equal_to_seialized) {
    commit_t commit{"message", 123, 456};
    auto serialized{serializer::serialize(commit)};
    auto deserialized{serializer::deserialize<commit_t>(serialized)};
    ASSERT_TRUE(bool(deserialized));
    EXPECT_TRUE(commit == *deserialized);
}

TEST(serializer, tree_deserialized_equal_to_serialized) {
    tree_t tree{{{"file1", 123}, {"file2", 234}}};
    auto serialized{serializer::serialize(tree)};
    auto deserialized{serializer::deserialize<tree_t>(serialized)};
    ASSERT_TRUE(bool(deserialized));
    EXPECT_TRUE(tree == *deserialized);
}

TEST(serializer, deserialize_error_handling) {
    auto deserialized{serializer::deserialize<blob_t>("asdf")};
    ASSERT_FALSE(bool(deserialized));
}

TEST(tree_equals, similar_trees_equal) {
    tree_t tree1{{{"name", 1}}};
    tree_t tree2{{{"name", 1}}};
    EXPECT_EQ(tree1, tree2);
}

TEST(tree_equals, different_amount_of_files_not_equal) {
    tree_t tree1{{{"name", 1}}};
    tree_t tree2{{{"name", 1}, {"name2", 2}}};
    EXPECT_FALSE(tree1 == tree2);
    EXPECT_FALSE(tree2 == tree1);
}

TEST(tree_equals, different_content_not_equal) {
    tree_t tree1{{{"name", 1}}};
    tree_t tree2{{{"name2", 1}}};
    EXPECT_FALSE(tree1 == tree2);
}
