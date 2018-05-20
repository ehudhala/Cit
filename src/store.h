#ifndef _CIT_STORE_H_
#define _CIT_STORE_H_

#include <map>
#include <memory>
#include <string>
#include <type_traits>

#include "boost/optional.hpp"
#include "boost/variant.hpp"

#include "objects.h"
#include "working_tree.h"

namespace cit {

using ref_name_t = std::string;
using ref_t = boost::variant<ref_name_t, hash_t>;

/**
 * Recursively loads names from the store to get the hash of the given ref.
 */
template <typename RefStore>
boost::optional<hash_t> get_ref_hash(const RefStore&, ref_t);

/**
 * Updates the hash the ref points to without changing ref names.
 * Only updates the deepest hash in the chain of names.
 * Returns the new ref.
 *  If the given ref is a hash, just returns the new hash.
 *  If the given ref is a name returns the given name, and updates the deep pointed hash.
 * e.g.:
 *      update_ref_hash(123, 456) -> 456
 *      update_ref_hash("ref_name", 456) -> "ref_name"
 */
template <typename RefStore>
ref_t update_ref_hash(RefStore&, ref_t, hash_t new_hash);

/**
 * Updates the hash the ref points to without changing ref names.
 * Only updates the deepest hash in the chain of names.
 */
template <typename RefStore>
void update_ref_deep_hash(RefStore&, ref_name_t, hash_t new_hash);

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

// TODO: we should add to objects many loads, probably as object methods
// e.g. commit.load_tree, tree.load_file

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

/**
 * Stores all the refs to commits.
 * Allows updating and loading them, most of the code using the ref should use
 * cit::get_ref_hash and cit::update_ref_hash
 */
class ref_store_t {
public:
    /**
     * Updates the ref with the given ref name to hold the new given ref.
     */
    void update(const ref_name_t&, ref_t);

    /**
     * Loads the ref content of the given ref name.
     * Returns either the loaded ref, or none if the ref doesn't point to anything.
     */
    boost::optional<ref_t> load(const ref_name_t&) const;
    // TODO: maybe return a const reference to the ref.

private:
    std::map<ref_name_t, ref_t> refs;
};

}

template <typename Index, typename RefStore>
struct store_t {
    using object_store = typename Index::object_store;
    store_t(Index index, RefStore refs, ref_t head);
    store_t(Index index, RefStore refs);

    object_store& get_objects();

    optional_hash get_head_hash() const;
    void update_head_hash(hash_t);

    Index index;
    RefStore refs;
    ref_t head;
    inline static const ref_t default_ref{"master"};
    // TODO: dummy first commit instead?
    // TODO: currently to get/update hash we have methods,
    // and to get/update the ref itself (e.g. to give it a name) we mutate head.
    // Maybe we should also expose methods to update the ref?
};

/**
 * Loads a commit and retrieves its tree.
 */
template <typename ObjectStore>
boost::optional<tree_t> load_tree(const ObjectStore&, hash_t commit_hash);

/**
 * Loads all the content of the given tree.
 */
template <typename ObjectStore>
boost::optional<tree_content_t> load_tree_content(const ObjectStore&, tree_t);

}

#endif
