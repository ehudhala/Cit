#ifndef _CIT_REPO_H_
#define _CIT_REPO_H_

#include "boost/optional.hpp"

#include "objects.h"
#include "store.h"
#include "working_tree.h"

namespace cit {
    
// Add, Commit, Checkout, Status.
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

// private:
    store_t store;
    working_tree_t working_tree;
};

}

#endif

#include "repo.inl"
