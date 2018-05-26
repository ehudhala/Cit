#ifndef _CIT_ALGORITHM_H_
#define _CIT_ALGORITHM_H_

#include <iostream>

#include "boost/optional.hpp"
#include "boost/optional/optional_io.hpp"
#include "boost/iterator/iterator_facade.hpp"

#include "objects.h"

namespace cit {

// TODO: doc, tests, dereference no commit.
template <typename ObjectStore>
class ancestor_iterator : public boost::iterator_facade<
          ancestor_iterator<ObjectStore>, commit_t,
          boost::single_pass_traversal_tag, commit_t> {
public:
    ancestor_iterator();
    explicit ancestor_iterator(commit_t commit, const ObjectStore& objects);

private:
    friend class boost::iterator_core_access;

    void increment();
    bool equal(ancestor_iterator<ObjectStore> const& other) const;
    commit_t dereference() const;

    boost::optional<commit_t> commit;
    boost::optional<const ObjectStore&> objects;
};

}

#endif
