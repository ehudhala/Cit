#include "gtest/gtest.h"

#include "objects.h"

#include "test_utils.h"

using namespace cit;

TEST(serializer, blob_deserialized_equal_to_serialized) {
    blob_t blob{"content"};
    auto serialized{serializer_t::serialize(blob)};
    auto deserialized{serializer_t::deserialize_blob(serialized)};
    ASSERT_TRUE(bool(deserialized));
    EXPECT_TRUE(blob_t{"content"} == *deserialized);
}

TEST(serializer, commit_deserialized_equal_to_serialized) {
    commit_t commit{"description"};
    auto serialized{serializer_t::serialize(commit)};
    auto deserialized{serializer_t::deserialize_commit(serialized)};
    ASSERT_TRUE(bool(deserialized));
    EXPECT_TRUE(commit_t{"description"} == *deserialized);
}

TEST(serializer, deserialize_error_handling) {
    auto deserialized{serializer_t::deserialize<blob_t>("asdf")};
    ASSERT_FALSE(bool(deserialized));
}
