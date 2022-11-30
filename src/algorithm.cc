#include <unordered_set>

#include "boost/functional/hash.hpp"

#include "algorithm.h"
#include "store.h"

namespace cit {

template <typename ObjectStore>
ancestor_iter<ObjectStore>::ancestor_iter()
    : commit(boost::none), objects(boost::none) {}

template <typename ObjectStore>
ancestor_iter<ObjectStore>::ancestor_iter(commit_t commit, const ObjectStore& objects)
    : commit(commit), objects(objects) {}

template <typename ObjectStore>
void ancestor_iter<ObjectStore>::increment() {
    if (!commit || !commit->parent_hash || !objects) {
        commit = boost::none;
    } else {
        commit = objects->template load<commit_t>(*commit->parent_hash);
    }
}

template <typename ObjectStore>
bool ancestor_iter<ObjectStore>::equal(ancestor_iter<ObjectStore> const& other) const {
    return commit == other.commit;
}

template <typename ObjectStore>
commit_t ancestor_iter<ObjectStore>::dereference() const {
    return *commit;
}

template <typename ObjectStore>
boost::optional<commit_t> get_branch(commit_t lhs, commit_t rhs, ObjectStore objects) {
    std::unordered_set<commit_t, boost::hash<commit_t>> lhs_ancestors;
    std::copy(ancestor_iter<ObjectStore>{lhs, objects},
              ancestor_iter<ObjectStore>{},
              std::inserter(lhs_ancestors, lhs_ancestors.end()));

    ancestor_iter<ObjectStore> rhs_iter{rhs, objects};
    for (; rhs_iter != ancestor_iter<ObjectStore>{}; rhs_iter++) {
        if (lhs_ancestors.find(*rhs_iter) != lhs_ancestors.end()) {
            return *rhs_iter;
        }
    }
    return boost::none;
}


using object_store = inmemory::object_store_t<serializer>;
template class ancestor_iter<object_store>;
template boost::optional<commit_t> get_branch(commit_t, commit_t, object_store);

}
