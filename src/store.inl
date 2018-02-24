#include "store.h"

#include "utils.h"

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
boost::optional<const std::string&> object_store_t<serializer_t>::raw_load(hash_t hash) const {
    return optional_read<hash_t, std::string, const std::string&>(objects_map, hash);
}

template <class serializer_t>
template <class Object>
boost::optional<Object> object_store_t<serializer_t>::deserialize_load(hash_t hash, deserialize_func<Object> deserialize) const {
    auto serialized = raw_load(hash);
    if (!serialized) {
        return boost::none;
    }
    return deserialize(*serialized);
}

template <class serializer_t>
template <class Object>
boost::optional<Object> object_store_t<serializer_t>::load(hash_t hash) const {
    deserialize_func<Object> deserialize = serializer_t::template deserialize<Object>;
    return deserialize_load(hash, deserialize);
}

template <class object_store_t>
hash_t index_t<object_store_t>::add(const name_t& name, const blob_t& blob) {
    hash_t hash = objects.store(blob);
    blob_names[name] = hash;
    return hash;
}

}

template <class index_t>
typename store_t<index_t>::object_store& store_t<index_t>::get_objects() {
    return index.objects;
}

}
