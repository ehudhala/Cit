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

    /**
     * Checkouts the repo to the given commit.
     * Changes the working tree and the index, and updates head.
     * Returns whether the checkout succeeded.
     * Checkout may fail when:
     *      The given hash is not a commit.
     */
    bool checkout(hash_t commit_hash);
    // TODO: we should make a commit_hash_t for static checks.

// private:
    Store store;
    WorkingTree working_tree;
};

/**
 * Loads a commit and retrieves its tree.
 */
template <typename ObjectStore>
boost::optional<tree_t> load_tree(const ObjectStore& objects, hash_t commit_hash);

}

#endif

#include "repo.inl"
