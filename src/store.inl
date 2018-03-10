#ifndef _CIT_STORE_INL_
#define _CIT_STORE_INL_

#include "store.h"

#include "utils.h"

namespace cit {

namespace inmemory {

template <typename Serializer>
hash_t object_store_t<Serializer>::store(const object_t& object) {
    std::string serialized = Serializer::serialize(object);
    hash_t hash = hash_func(serialized);
    objects_map[hash] = serialized;
    return hash;
}

template <typename Serializer>
boost::optional<const std::string&> object_store_t<Serializer>::raw_load(hash_t hash) const {
    return optional_read<hash_t, std::string, const std::string&>(objects_map, hash);
}

template <typename Serializer>
template <typename Object>
boost::optional<Object> object_store_t<Serializer>::deserialize_load(hash_t hash, deserialize_func<Object> deserialize) const {
    auto serialized = raw_load(hash);
    if (!serialized) {
        return boost::none;
    }
    return deserialize(*serialized);
}

template <typename Serializer>
template <typename Object>
boost::optional<Object> object_store_t<Serializer>::load(hash_t hash) const {
    deserialize_func<Object> deserialize = Serializer::template deserialize<Object>;
    return deserialize_load(hash, deserialize);
}

template <typename ObjectStore>
hash_t index_t<ObjectStore>::add(const name_t& name, const blob_t& blob) {
    hash_t hash = objects.store(blob);
    files.push_back({name, hash});
    return hash;
}

}

template <typename Index>
typename store_t<Index>::object_store& store_t<Index>::get_objects() {
    return index.objects;
}

}

#endif
