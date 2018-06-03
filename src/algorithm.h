#ifndef _CIT_ALGORITHM_H_
#define _CIT_ALGORITHM_H_

#include <iostream>

#include "boost/optional.hpp"
#include "boost/optional/optional_io.hpp"
#include "boost/iterator/iterator_facade.hpp"

#include "objects.h"

namespace cit {

/**
 * An iterator over commit ancestors.
 * Holds (and dereferences) a commit, and on iteration updates to the commits parent.
 * A default constructed ancestor_iter holds no commit - which is the end state of all iterations.
 */
template <typename ObjectStore>
class ancestor_iter : public boost::iterator_facade<
          ancestor_iter<ObjectStore>, commit_t,
          boost::single_pass_traversal_tag, commit_t> {
public:
    /**
     * Holds an empty commit, can be used as an end iterator.
     */
    ancestor_iter();

    /**
     * Holds a commit and on ObjectStore to access the commits parents.
     */
    explicit ancestor_iter(commit_t commit, const ObjectStore& objects);

private:
    friend class boost::iterator_core_access;

    /**
     * Incrementing a ancestor_iter is updating the commit to its parent.
     */
    void increment();

    /**
     * Two ancestor_iters are equal if they point to the same commit.
     * They are also equal if they are both none.
     */
    bool equal(ancestor_iter<ObjectStore> const& other) const;

    /**
     * Dereferencinig is returning a copy of the current commit.
     * If the current commit is none you must not dereference it.
     *  This can be easily checked with equality against a default-constructed ancestor_iter.
     */
    commit_t dereference() const;

    boost::optional<commit_t> commit;
    boost::optional<const ObjectStore&> objects;
};

}

#endif
