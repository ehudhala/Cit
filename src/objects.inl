#include <istream>
#include <sstream>
#include <memory>

#include "objects.h"

namespace cit {

std::unique_ptr<std::istream> inmemory_blob_t::get_content() const {
    return std::make_unique<std::istringstream>(content);
}

std::unique_ptr<std::istream> commit_t::get_content() const {
    return std::make_unique<std::istringstream>(description);
}

hash_t inmemory_object_store_t::save(object_t& object) {
    hash_t hash = hash_func(object);
    return hash;
}

}
