#include <istream>
#include <sstream>
#include <memory>
#include <optional>

#include "objects.h"

namespace cit {

struct serializer {
    std::string operator()(const blob_t& blob) const {
        return blob.content;
    }
    std::string operator()(const commit_t& commit) const {
        return commit.description;
    }
};

std::string serialize(const object_t& object) {
    return boost::apply_visitor(serializer{}, object);
}

hash_t inmemory_object_store_t::save(const object_t& object) {
    std::string serialized = serialize(object);
    hash_t hash = hash_func(serialized);
    objects_map[hash] = serialized;
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
