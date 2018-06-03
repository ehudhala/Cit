#include <iostream>
#include <iterator>

#include "gtest/gtest.h"

#include "algorithm.h"
#include "test_utils.h"

using namespace cit;

using ancestor_iter_t = ancestor_iter<object_store_t>;


TEST(ancestor_iter, increment_points_to_parent) {
    auto objects{inc_object_store()};
    commit_t parent_commit{"parent message", 0};
    auto parent_hash = objects.store(parent_commit);
    commit_t child_commit{"child message", parent_hash, 0};

    ancestor_iter_t iter{child_commit, objects};
    iter++;
    ancestor_iter_t parent_iter{parent_commit, objects};
    EXPECT_EQ(parent_iter, iter);
}


TEST(ancestor_iter, standard_algorithms) {
    auto objects{inc_object_store()};
    commit_t parent_commit{"parent message", 0};
    auto parent_hash = objects.store(parent_commit);
    commit_t child_commit{"child message", parent_hash, 0};

    ancestor_iter_t iterator{child_commit, objects};
    std::vector<commit_t> ancestors;
    std::copy(iterator, ancestor_iter_t{}, std::back_inserter(ancestors));

    ASSERT_EQ(2, ancestors.size());
    EXPECT_EQ(child_commit, ancestors[0]);
    EXPECT_EQ(parent_commit, ancestors[1]);
}
