#include <istream>
#include <sstream>
#include <memory>
#include <optional>

#include "objects.h"

namespace cit {

std::string blob_t::serialize() const {
    return content;
}

std::string commit_t::serialize() const {
    return description;
}

hash_t inmemory_object_store_t::save(const object_t& object) {
    hash_t hash = hash_func(object.serialize());
    objects_map[hash] = object.serialize();
    return hash;
}

std::optional<commit_t> inmemory_object_store_t::load_commit(hash_t hash) const {
    auto obj_it = objects_map.find(hash);
    if (obj_it != objects_map.end()) {
        // TODO: return commit;
        return commit_t{"asdf"};
    }
    return std::nullopt;
}

}
