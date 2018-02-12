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

namespace inmemory {

using hash_func_t = std::function<hash_t(const std::string&)>;

/**
 * The heavy storage of Cit.
 * Stores all the objects (blobs, commits, trees).
 * Has a simple api - store and load.
 * We have many loads in order to load back the type stored.
 * This object store stores all the objects in memory.
 */
template <class serializer_t>
class object_store_t {
public:
    /**
     * The hash function to use, and the serializer to use
     * are both dynamically injected to the store.
     */
    object_store_t(hash_func_t hash_func, serializer_t serializer)
        : hash_func(hash_func), serializer(serializer) {}

    /**
     * Stores any object.
     * Returns the hash (id) of the stored object.
     */
    hash_t store(const object_t&);

    /**
     * Loads a blob from the storage.
     * Returns the blob on success, and empty on either:
     * * The object is not found.
     * * The object cannot be deserialized as a blob.
     */
    optional_blob load_blob(hash_t) const;

    /**
     * Loads a commit from the storage.
     * Returns the commit on success, and empty on either:
     * * The object is not found.
     * * The object cannot be deserialized as a commit.
     */
    optional_commit load_commit(hash_t) const;

    /**
     * Loads an object from the storage.
     * Returns the object on success, and empty when it is not found.
     */
    boost::optional<const std::string&> load_object(hash_t hash) const;
private:
    hash_func_t hash_func;
    serializer_t serializer;

    std::map<hash_t, std::string> objects_map;
};

// TODO: index, then support add.

using blob_names_t = std::map<std::string, hash_t>;

template <class object_store_t, class blob_t>
class index_t {
    using objecty_store = object_store_t;
public:
    index_t(object_store_t&);
    hash_t add(const std::string& name, blob_t);
private:
    blob_names_t blob_names;
    object_store_t& object_store;
};

}

template <class index_t, class object_store_t, class ref_store_t>
class store_t {
public:
    static_assert(std::is_same<typename index_t::object_store, object_store_t>::value, 
            "The index should use the same object_store as the store.");
    index_t index;
    object_store_t objects;
};

}

#endif

#include "store.inl"
