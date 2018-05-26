#include "algorithm.h"
#include "store.h"

namespace cit {

template <typename ObjectStore>
ancestor_iterator<ObjectStore>::ancestor_iterator()
    : commit(boost::none), objects(boost::none) {}

template <typename ObjectStore>
ancestor_iterator<ObjectStore>::ancestor_iterator(commit_t commit, const ObjectStore& objects)
    : commit(commit), objects(objects) {}

template <typename ObjectStore>
void ancestor_iterator<ObjectStore>::increment() {
    if (!commit || !commit->parent_hash || !objects) {
        commit = boost::none;
    } else {
        commit = objects->template load<commit_t>(*commit->parent_hash);
    }
}

template <typename ObjectStore>
bool ancestor_iterator<ObjectStore>::equal(ancestor_iterator<ObjectStore> const& other) const {
    return commit == other.commit;
}

template <typename ObjectStore>
commit_t ancestor_iterator<ObjectStore>::dereference() const {
    if (bool(commit)) {
        return *commit;
    }
    return commit_t{}; // TODO: what to do when we dereference an invalid iterator?
}

template class ancestor_iterator<inmemory::object_store_t<serializer>>;

}
