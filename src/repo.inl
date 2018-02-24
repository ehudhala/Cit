#include "repo.h"

namespace cit {

template <class store_t, class working_tree_t>
optional_hash repo_t<store_t, working_tree_t>::add(const name_t& name) {
    auto content = working_tree.read(name);
    if (!content) {
        return boost::none;
    }
    blob_t blob{*content};
    return store.index.add(name, blob);
}

}
