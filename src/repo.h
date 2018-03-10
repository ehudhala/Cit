#ifndef _CIT_REPO_H_
#define _CIT_REPO_H_

#include "boost/optional.hpp"

#include "objects.h"
#include "store.h"
#include "working_tree.h"

namespace cit {
    
// Add, Commit, Checkout, Status.
template <typename Store, typename WorkingTree>
class repo_t {
public:
    repo_t(Store store, WorkingTree working_tree)
        : store(store), working_tree(working_tree) {}

    /**
     * Adds a file to the index.
     * Returns false if the file doesn't exist.
     */
    optional_hash add(const name_t&);

    /**
     * Commits the current index.
     * Creates a commit object, and updates the current ref.
     */
    hash_t commit(const std::string& message);

// private:
    Store store;
    WorkingTree working_tree;
};

}

#endif

#include "repo.inl"
