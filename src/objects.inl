#include <istream>
#include <sstream>
#include <memory>

#include "objects.h"

namespace cit {

std::unique_ptr<std::istream> inmemory_blob_t::get_content() const {
    return std::make_unique<std::istringstream>(content);
}

std::unique_ptr<std::istream> inmemory_commit_t::get_content() const {
    return std::make_unique<std::istringstream>(description);
}

template <class commit_t>
hash_t inmemory_object_store_t<commit_t>::save(std::unique_ptr<object_t> object) {
    hash_t hash = hash_func(*object);
    return hash;
}

template <class commit_t>
commit_t inmemory_object_store_t<commit_t>::load_commit(hash_t) const {
    return commit_t{"asdf"};
}

}
