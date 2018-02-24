#include "gtest/gtest.h"

#include "objects.h"

#include "test_utils.h"

using namespace cit;

TEST(serializer, blob_deserialized_equal_to_serialized) {
    blob_t blob{"content"};
    auto serialized{serializer_t::serialize(blob)};
    auto deserialized{serializer_t::deserialize<blob_t>(serialized)};
    ASSERT_TRUE(bool(deserialized));
    EXPECT_TRUE(blob_t{"content"} == *deserialized);
}

TEST(serializer, commit_deserialized_equal_to_serialized) {
    commit_t commit{"description"};
    auto serialized{serializer_t::serialize(commit)};
    auto deserialized{serializer_t::deserialize<commit_t>(serialized)};
    ASSERT_TRUE(bool(deserialized));
    EXPECT_TRUE(commit_t{"description"} == *deserialized);
}
// TODO: test failure.
