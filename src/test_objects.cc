#include "gtest/gtest.h"

#include "objects.h"

#include "test_utils.h"

using namespace cit;

TEST(serializer_serialize, blob) {
    blob_t blob{"a1"};
    EXPECT_EQ("a1", serializer.serialize(blob));
}

TEST(serializer_serialize, commit) {
    commit_t commit{"a2"};
    EXPECT_EQ("a2", serializer.serialize(commit));
}

TEST(serializer_deserialize, blob) {
    auto blob{serializer.deserialize_blob("b1")};
    ASSERT_TRUE(bool(blob));
    EXPECT_TRUE(blob_t{"b1"} == *blob);
}

TEST(serializer_deserialize, commit) {
    auto commit{serializer.deserialize_commit("b2")};
    ASSERT_TRUE(bool(commit));
    EXPECT_TRUE(commit_t{"b2"} == *commit);
}

