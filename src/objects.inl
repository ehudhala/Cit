#include <istream>
#include <sstream>
#include <memory>
#include <optional>

#include "objects.h"

namespace cit {

struct serialize_visitor {
    std::string operator()(const blob_t& blob) const {
        return blob.content;
    }
    std::string operator()(const commit_t& commit) const {
        return commit.description;
    }
};

std::string serializer_t::serialize(const object_t& object) {
    return boost::apply_visitor(serialize_visitor{}, object);
}

template <class serializer_t>
hash_t inmemory_object_store_t<serializer_t>::save(const object_t& object) {
    std::string serialized = serializer.serialize(object);
    hash_t hash = hash_func(serialized);
    objects_map[hash] = serialized;
    return hash;
}

template <class serializer_t>
optional<commit_t> inmemory_object_store_t<serializer_t>::load_commit(hash_t hash) const {
    auto obj_it = objects_map.find(hash);
    if (obj_it != objects_map.end()) {
        // TODO: return commit;
        return commit_t{"asdf"};
    }
    return nullopt;
}

}
