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

}

#endif
