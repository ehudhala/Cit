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

template class ancestor_iter<inmemory::object_store_t<serializer>>;

}
