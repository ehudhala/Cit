#ifndef _CIT_STORE_H_
#define _CIT_STORE_H_

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
template <typename Object>
using deserialize_func = std::function<boost::optional<Object>(const std::string&)>;

/**
 * The heavy storage of Cit.
 * Stores all the objects (blobs, commits, trees).
 * Has a simple api - store and load.
 * We have many loads in order to load back the type stored.
 * This object store stores all the objects in memory.
 */
template <typename Serializer>
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

/**
 * Loads a commit and retrieves its tree.
 */
template <typename ObjectStore>
boost::optional<tree_t> load_tree(const ObjectStore& objects, hash_t commit_hash);

/**
 * Used as the index for Cit.
 * Contains the currently staged files.
 * Adding a file to the index stores the blob, and saves its new state.
 */
template <typename ObjectStore>
class index_t {
public:
    // TODO: rename object_store according to conventions.
    using object_store = ObjectStore;
    /**
     * The index is injected with the object_store it should store objects to.
     */
    index_t(ObjectStore objects) : objects(objects) {}

    /**
     * Adds a file to the index.
     * Saves it both to the object_store, and to the index.
     */
    hash_t add(const name_t&, const blob_t&);

    /**
     * Updates the index to hold the given files and hashes.
     */
    void update(std::vector<file_t>);

    /**
     * The index contents.
     * Contains all the current files in the index.
     * TODO: this should be private, externally we should have a method to access.
     */
    std::vector<file_t> files;

    ObjectStore objects;
};

}

template <typename Index>
struct store_t {
    using object_store = typename Index::object_store;
    store_t(Index index) : index(index) {}

    object_store& get_objects();
    Index index;
    optional_hash head; // false before the first commit.
    // TODO: dummy first commit instead?
};

}

#endif

#include "store.inl"
