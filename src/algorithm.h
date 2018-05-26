#ifndef _CIT_ALGORITHM_H_
#define _CIT_ALGORITHM_H_

#include <iostream>

#include "boost/optional.hpp"
#include "boost/optional/optional_io.hpp"
#include "boost/iterator/iterator_facade.hpp"

#include "objects.h"

namespace cit {

// TODO: doc, refactor to .cc file
template <typename ObjectStore>
class ancestor_iterator : public boost::iterator_facade<
          ancestor_iterator<ObjectStore>, commit_t,
          boost::single_pass_traversal_tag, 
          commit_t> {
public:
    ancestor_iterator() : commit(boost::none), objects(boost::none) {}
    explicit ancestor_iterator(commit_t commit, const ObjectStore& objects) 
        : commit(commit), objects(objects) {} // commit_t &?

private:
    friend class boost::iterator_core_access;

    void increment() {
        if (!commit || !commit->parent_hash || !objects) {
            commit = boost::none;
        } else {
            commit = objects->template load<commit_t>(*commit->parent_hash);
        }
    };

    bool equal(ancestor_iterator const& other) const {
        return commit == other.commit;
    }

    commit_t dereference() const {
        if (bool(commit)) {
            return *commit;
        }
        return commit_t{}; // TODO: what to do when we dereference an invalid iterator?
    }

    boost::optional<commit_t> commit;
    boost::optional<const ObjectStore&> objects;
};

}

#endif
