#pragma once
#ifndef _STOREH
#define _STOREH

#include <map>
#include <memory>
#include <string>
#include <type_traits>

#include "boost/optional.hpp"

#include "objects.h"
#include "working_tree.h"

namespace cit {

namespace inmemory {

using hash_func_t = std::function<hash_t(const std::string&)>;
template <class object>
using deserialize_func = std::function<boost::optional<object>(const std::string&)>;

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
    object_store_t(hash_func_t hash_func) : hash_func(hash_func) {}

    /**
     * Stores any object.
     * Returns the hash (id) of the stored object.
     */
    hash_t store(const object_t&);

    /**
     * Loads an object from the storage.
     * Returns the object on success, and empty on either:
     * * The object is not found.
     * * The object cannot be deserialized.
     */
    template <class Object>
    boost::optional<Object> load(hash_t hash) const;

    /**
     * Loads an object from the storage.
     * Returns the object on success, and empty when it is not found.
     */
    boost::optional<const std::string&> raw_load(hash_t hash) const;

private:
    /**
     * Loads an object from the storage and deserializes it.
     * Returns the object on success, and empty on either:
     * * The object is not found.
     * * The object cannot be deserialized.
     */
    template <class Object>
    boost::optional<Object> deserialize_load(hash_t hash, deserialize_func<Object>) const;

    hash_func_t hash_func;

    std::map<hash_t, std::string> objects_map;
};

using blob_names_t = std::map<std::string, hash_t>;

/**
 * Used as the index for Cit.
 * Contains the currently staged files.
 * Adding a file to the index stores the blob, and saves its new state.
 */
template <class object_store_t>
class index_t {
public:
    using object_store = object_store_t;
    /**
     * The index is injected with the object_store it should store objects to.
     */
    index_t(object_store_t objects) : objects(objects) {}

    /**
     * Adds a file to the index.
     * Saves it both to the object_store, and to the index.
     */
    hash_t add(const name_t&, const blob_t&);

    /**
     * The index contents.
     * Contains all the current files in the index.
     */
    blob_names_t blob_names;

    object_store_t objects;
};

}

template <class index_t>
struct store_t {
    using object_store = typename index_t::object_store;
    store_t(index_t index)
        : index(index) {}

    object_store& get_objects();

    index_t index;
    // TODO: ref_store.
};

}

#endif

#include "store.inl"
