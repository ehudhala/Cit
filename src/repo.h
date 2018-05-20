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
     *     * The given hash is not a commit.
     *     * The commit isn't valid (no tree, tree doesn't have content).
     */
    bool checkout(hash_t commit_hash);
    // TODO: we should make a commit_hash_t for compiler validation.

    /**
     * Checks out the given ref:
     *     * Updates head to the given name, tracking it from now.
     *     * Checks out the commit the ref points to.
     * Returns whether the checkout succeeded or not.
     * Checkout may fail when:
     *     * Regular checkout may fail.
     *     * The given name doesn't point to a hash.
     */
    bool checkout(ref_name_t);

    /**
     * Creates a new branch pointing to the current head.
     * Returns whether the new branch was created.
     * Will not create a new branch when the repo's head doesn't point to a valid commit.
     */
    bool branch(ref_name_t);

// private:
    Store store;
    WorkingTree working_tree;
};

}

#endif
