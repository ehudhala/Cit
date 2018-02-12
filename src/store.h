#pragma once
#ifndef _STOREH
#define _STOREH

#include <map>
#include <memory>
#include <string>
#include <type_traits>

#include "boost/optional.hpp"

#include "objects.h"

namespace cit {

// Should be in namespace inmemory.
template <class serializer_t>
class inmemory_object_store_t {
public:
    inmemory_object_store_t(
            std::function<hash_t(const std::string&)> hash_func,
            serializer_t serializer)
        : hash_func(hash_func), serializer(serializer) {}

    hash_t save(const object_t&);

    boost::optional<const std::string&> load_object(hash_t hash) const;
    boost::optional<commit_t> load_commit(hash_t) const;
    boost::optional<blob_t> load_blob(hash_t) const;
private:
    std::function<hash_t(const std::string&)> hash_func;
    serializer_t serializer;

    std::map<hash_t, std::string> objects_map;
};

// TODO: inmemory_index, then support add.
// Maybe its time to split the code up to files.

using blob_names_t = std::map<std::string, hash_t>;

template <class object_store_t, class blob_t>
class inmemory_index_t {
    using objecty_store = object_store_t;
public:
    inmemory_index_t(object_store_t&);
    hash_t add(const std::string& name, blob_t);
private:
    blob_names_t blob_names;
    object_store_t& object_store;
};

class inmemory_ref_store_t {
public:
    hash_t get_hash(const ref_t&);
    hash_t set_ref(const ref_t&, hash_t);

    ref_t get_head();
    void set_head(const ref_t&);
};

template <class index_t, class object_store_t, class ref_store_t>
class store_t {
public:
    static_assert(std::is_same<typename index_t::object_store, object_store_t>::value, 
            "The index should use the same object_store as the store.");
    index_t index;
    object_store_t objects;
    ref_store_t refs;
};

}

#endif

#include "store.inl"
