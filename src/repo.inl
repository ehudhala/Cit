#ifndef _CIT_REPO_INL_
#define _CIT_REPO_INL_

#include "repo.h"

namespace cit {

template <typename Store, typename WorkingTree>
optional_hash repo_t<Store, WorkingTree>::add(const name_t& name) {
    auto content = working_tree.read(name);
    if (!content) {
        return boost::none;
    }
    blob_t blob{*content};
    return store.index.add(name, blob);
}

template <typename Store, typename WorkingTree>
hash_t repo_t<Store, WorkingTree>::commit(const std::string& message) {
    tree_t tree{store.index.files};
    hash_t tree_hash = store.get_objects().store(tree);
    commit_t commit(message, store.head, tree_hash);
    hash_t commit_hash = store.get_objects().store(commit);
    store.head = commit_hash;
    return commit_hash;
}

// TODO: checkout MUST be atommic, we don't want corruptions...
// for the first phase it's not atomic though.
template <typename Store, typename WorkingTree>
bool repo_t<Store, WorkingTree>::checkout(hash_t commit_hash) {
    auto tree = load_tree(store.get_objects(), commit_hash);
    if (!tree) {
        return false;
    }
    store.index.update((*tree).files);

    auto tree_content = load_tree_content(store.get_objects(), *tree);
    if (!tree_content) {
        return false;
    }
    update_working_tree(working_tree, *tree_content);
    return false;
}

}

#endif
