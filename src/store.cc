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

template <typename ObjectStore>
void index_t<ObjectStore>::update(std::vector<file_t> new_files) {
    files = std::move(new_files);
}

void ref_store_t::update(const ref_name_t& ref_name, ref_t ref) {
    refs[ref_name] = ref;
}

boost::optional<ref_t> ref_store_t::load(const ref_name_t& ref_name) const {
    return optional_read<ref_name_t, ref_t, ref_t>(refs, ref_name);
}

}

template <typename Index, typename RefStore>
typename store_t<Index, RefStore>::object_store& store_t<Index, RefStore>::get_objects() {
    return index.objects;
}

template <typename Index, typename RefStore>
RefStore& store_t<Index, RefStore>::get_refs() {
    return refs;
}

template <typename ObjectStore>
boost::optional<tree_t> load_tree(const ObjectStore& objects, hash_t commit_hash) {
    auto commit = objects.template load<commit_t>(commit_hash);
    if (!commit) {
        return boost::none;
    }
    return objects.template load<tree_t>((*commit).tree_hash);
}

template <typename ObjectStore>
boost::optional<tree_content_t> load_tree_content(const ObjectStore& objects, tree_t tree) {
    tree_content_t content;
    for (const auto& file : tree.files) {
        auto blob = objects.template load<blob_t>(file.blob_hash);
        if (!blob) {
            return boost::none;
        }
        content[file.name] = (*blob).content;
    }
    return content;
}

// Instantiations
using object_store = inmemory::object_store_t<serializer>;
using index = inmemory::index_t<object_store>;
namespace inmemory {
template class object_store_t<serializer>;
template class index_t<object_store>;
}
template struct store_t<index, inmemory::ref_store_t>;

template boost::optional<tree_t> load_tree(const object_store&, hash_t commit_hash);
template boost::optional<tree_content_t> load_tree_content(const object_store&, tree_t);

template class inmemory::object_store_t<failing_deserializtion>; // For tests.
template boost::optional<blob_t> inmemory::object_store_t<failing_deserializtion>::load(hash_t) const;

}

#endif
