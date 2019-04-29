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
    commit_t commit{"description"};
    auto serialized{serializer::serialize(commit)};
    auto deserialized{serializer::deserialize<commit_t>(serialized)};
    ASSERT_TRUE(bool(deserialized));
    EXPECT_TRUE(commit_t{"description"} == *deserialized);
}

TEST(serializer, commit_with_parent_deserialized_equal_to_serialized) {
    commit_t commit{"description", 123};
    auto serialized{serializer::serialize(commit)};
    auto deserialized{serializer::deserialize<commit_t>(serialized)};
    ASSERT_TRUE(bool(deserialized));
    EXPECT_TRUE(commit_t("description", 123) == *deserialized);
}

TEST(serializer, deserialize_error_handling) {
    auto deserialized{serializer::deserialize<blob_t>("asdf")};
    ASSERT_FALSE(bool(deserialized));
}
