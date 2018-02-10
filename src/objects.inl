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

std::string serializer_t::serialize(const object_t& object) const {
    return boost::apply_visitor(serialize_visitor{}, object);
}

boost::optional<blob_t> serializer_t::deserialize_blob(const std::string& serialized) const {
    return blob_t{serialized};
}

boost::optional<commit_t> serializer_t::deserialize_commit(const std::string& serialized) const {
    return commit_t{serialized};
    // TODO: make deserializtion more complex, and add tests for failing,
    // and for failing to load_commit.
}

template <class serializer_t>
hash_t inmemory_object_store_t<serializer_t>::save(const object_t& object) {
    std::string serialized = serializer.serialize(object);
    hash_t hash = hash_func(serialized);
    objects_map[hash] = serialized;
    return hash;
}


template <class serializer_t>
boost::optional<const std::string&> inmemory_object_store_t<serializer_t>::load_object(hash_t hash) const {
    auto obj_it = objects_map.find(hash);
    if (obj_it != objects_map.end()) {
        return obj_it->second;
    }
    return boost::none;
}

template <class serializer_t>
boost::optional<commit_t> inmemory_object_store_t<serializer_t>::load_commit(hash_t hash) const {
    auto serialized = load_object(hash);
    if (!serialized) {
        return boost::none;
    }
    return serializer.deserialize_commit(*serialized);
}

template <class serializer_t>
boost::optional<blob_t> inmemory_object_store_t<serializer_t>::load_blob(hash_t hash) const {
    boost::optional<const std::string&> serialized = load_object(hash);
    if (!serialized) {
        return boost::none;
    }
    return serializer.deserialize_blob(*serialized);
}

}
