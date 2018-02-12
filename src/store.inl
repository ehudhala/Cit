#include "store.h"

namespace cit {

namespace inmemory {

template <class serializer_t>
hash_t object_store_t<serializer_t>::store(const object_t& object) {
    std::string serialized = serializer.serialize(object);
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
optional_commit object_store_t<serializer_t>::load_commit(hash_t hash) const {
    auto serialized = load_object(hash);
    if (!serialized) {
        return boost::none;
    }
    return serializer.deserialize_commit(*serialized);
}

template <class serializer_t>
optional_blob object_store_t<serializer_t>::load_blob(hash_t hash) const {
    boost::optional<const std::string&> serialized = load_object(hash);
    if (!serialized) {
        return boost::none;
    }
    return serializer.deserialize_blob(*serialized);
}

}
}
