#include "store.h"

namespace cit {

namespace inmemory {

template <class serializer_t>
hash_t object_store_t<serializer_t>::store(const object_t& object) {
    std::string serialized = serializer_t::serialize(object);
    hash_t hash = hash_func(serialized);
    objects_map[hash] = serialized;
    return hash;
}

template <class serializer_t>
boost::optional<const std::string&> object_store_t<serializer_t>::load_object(hash_t hash) const {
    auto obj_it = objects_map.find(hash);
    if (obj_it != objects_map.end()) {
        return obj_it->second;
    }
    return boost::none;
}

template <class serializer_t>
template <class object>
boost::optional<object> object_store_t<serializer_t>::load(hash_t hash, deserialize_func<object> deserialize) const {
    auto serialized = load_object(hash);
    if (!serialized) {
        return boost::none;
    }
    return deserialize(*serialized);
}

template <class serializer_t>
optional_commit object_store_t<serializer_t>::load_commit(hash_t hash) const {
    deserialize_func<commit_t> deserialize = serializer_t::deserialize_commit;
    return load(hash, deserialize);
}

template <class serializer_t>
optional_blob object_store_t<serializer_t>::load_blob(hash_t hash) const {
    deserialize_func<blob_t> deserialize = serializer_t::deserialize_blob;
    return load(hash, deserialize);
}

}
}
