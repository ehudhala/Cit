#ifndef _CIT_STORE_INL_
#define _CIT_STORE_INL_

#include "boost/hana/functional/overload.hpp"

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

template <typename RefStore>
boost::optional<hash_t> get_ref_hash(const RefStore& refs, ref_t ref) {
    auto visitor = boost::hana::overload(
        [&refs](const hash_t& hash) -> boost::optional<hash_t> {
            return hash;
        },
        [&refs](const ref_name_t& ref_name) -> boost::optional<hash_t> {
            auto loaded_ref{refs.load(ref_name)};
            if (!loaded_ref) {
                return boost::none;
            }
            return get_ref_hash(refs, *loaded_ref);
        }
    );
    return boost::apply_visitor(visitor, ref);
}

// TODO: maybe this should be two overloaded functions.
template <typename RefStore>
ref_t update_ref_hash(RefStore& refs, ref_t ref, hash_t new_hash) {
    auto visitor = boost::hana::overload(
        [&refs, new_hash](const hash_t&) -> ref_t {
            return new_hash;
        },
        [&refs, new_hash](const ref_name_t& ref_name) -> ref_t {
            update_ref_deep_hash(refs, ref_name, new_hash);
            return ref_name;
        }
    );
    return boost::apply_visitor(visitor, ref);
}

template <typename RefStore>
void update_ref_deep_hash(RefStore& refs, ref_name_t ref_name, hash_t new_hash) {
    auto son_ref{refs.load(ref_name)};
    if (!son_ref) {
        refs.update(ref_name, new_hash);
    }
    else {
        auto visitor = boost::hana::overload(
            [&refs, &ref_name, new_hash](const hash_t&) {
                refs.update(ref_name, new_hash);
            },
            [&refs, new_hash](const ref_name_t& son_name) {
                update_ref_deep_hash(refs, son_name, new_hash);
            }
        );
        boost::apply_visitor(visitor, *son_ref);
    }
}

template <typename Index, typename RefStore>
store_t<Index, RefStore>::store_t(Index index, RefStore refs, ref_t head)
    : index(index), refs(refs), head(head) {}

template <typename Index, typename RefStore>
store_t<Index, RefStore>::store_t(Index index, RefStore refs)
    : index(index), refs(refs), head(default_ref) {}

template <typename Index, typename RefStore>
typename store_t<Index, RefStore>::object_store& store_t<Index, RefStore>::get_objects() {
    return index.objects;
}

template <typename Index, typename RefStore>
optional_hash store_t<Index, RefStore>::get_head_hash() const {
    return get_ref_hash(refs, head);
}

template <typename Index, typename RefStore>
void store_t<Index, RefStore>::update_head_hash(hash_t new_hash) {
    head = update_ref_hash(refs, head, new_hash);
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

template boost::optional<hash_t> get_ref_hash(const inmemory::ref_store_t&, ref_t);

template ref_t update_ref_hash(inmemory::ref_store_t&, ref_t, hash_t);
template void update_ref_deep_hash(inmemory::ref_store_t&, ref_name_t, hash_t);

}

#endif
