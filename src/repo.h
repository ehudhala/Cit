#pragma once
#ifndef _REPOH
#define _REPOH

#include "boost/optional.hpp"

#include "objects.h"
#include "store.h"
#include "working_tree.h"

namespace cit {
    
using optional_hash = boost::optional<hash_t>;

// Add, Commit, Checkout, Status.
// We need WorkingTree.
template <class store_t, class working_tree_t>
class repo_t {
public:
    repo_t(store_t store, working_tree_t working_tree)
        : store(store), working_tree(working_tree) {}

    /**
     * Adds a file to the index.
     * Returns false if the file doesn't exist.
     */
    optional_hash add(const name_t&);

private:
    store_t store;
    working_tree_t working_tree;
};

}

#endif

#include "repo.inl"
