// TODO: refactor this test file with gtest fixtures.

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


TEST(get_branch, finds_branch) {
    auto objects{inc_object_store()};
    commit_t parent_commit{"parent message", 0};
    auto parent_hash = objects.store(parent_commit);
    commit_t lhs_commit{"lhs message", parent_hash, 0};
    commit_t rhs_commit{"rhs message", parent_hash, 0};

    auto branch = get_branch(lhs_commit, rhs_commit, objects);
    ASSERT_TRUE(bool(branch));
    EXPECT_EQ(parent_commit, *branch);
}


TEST(get_branch, finds_lhs_is_branch) {
    auto objects{inc_object_store()};
    commit_t parent_commit{"parent message", 0};
    auto parent_hash = objects.store(parent_commit);
    commit_t child_commit{"rhs message", parent_hash, 0};

    auto branch = get_branch(parent_commit, child_commit, objects);
    ASSERT_TRUE(bool(branch));
    EXPECT_EQ(parent_commit, *branch);
}

TEST(get_branch, finds_rhs_is_branch) {
    auto objects{inc_object_store()};
    commit_t parent_commit{"parent message", 0};
    auto parent_hash = objects.store(parent_commit);
    commit_t child_commit{"rhs message", parent_hash, 0};

    auto branch = get_branch(child_commit, parent_commit, objects);
    ASSERT_TRUE(bool(branch));
    EXPECT_EQ(parent_commit, *branch);
}

TEST(get_branch, returns_none_on_different_linages) {
    auto objects{inc_object_store()};

    commit_t lhs_parent_commit{"lhs parent message", 0};
    auto lhs_parent_hash = objects.store(lhs_parent_commit);
    commit_t lhs_commit{"lhs message", lhs_parent_hash, 0};

    commit_t rhs_parent_commit{"rhs parent message", 0};
    auto rhs_parent_hash = objects.store(rhs_parent_commit);
    commit_t rhs_commit{"rhs message", rhs_parent_hash, 0};

    auto branch = get_branch(lhs_commit, rhs_commit, objects);
    ASSERT_FALSE(bool(branch));
}
