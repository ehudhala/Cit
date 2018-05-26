#include <iostream>
#include <iterator>

#include "gtest/gtest.h"

#include "algorithm.h"
#include "test_utils.h"

using namespace cit;

using ancestor_iterator_t = ancestor_iterator<object_store_t>;

TEST(ancestor_iterator, iteration) {
    auto objects{inc_object_store()};
    auto parent_hash = objects.store(commit_t{});
    commit_t child_commit{"", parent_hash, 0};

    ancestor_iterator_t iterator{child_commit, objects};
    std::vector<commit_t> ancestors;
    std::copy(iterator, ancestor_iterator_t(), std::back_inserter(ancestors));

    EXPECT_EQ(child_commit, ancestors[0]);
    EXPECT_EQ(commit_t{}, ancestors[1]);
}

