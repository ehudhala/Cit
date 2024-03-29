#ifndef _CIT_OBJECTS_H_
#define _CIT_OBJECTS_H_

#include <string>
#include <vector>

#include "boost/variant.hpp"
#include "boost/optional.hpp"

#include "working_tree.h"

namespace cit {

using hash_t = std::size_t;
using optional_hash = boost::optional<hash_t>;

/*
 * TODO: decide on the ownership model of the content stream.
 * * For blobs:
 *     * The store owns the content:
 *         * Hold and return a ref
 *         * Hold and return a weak_ptr
 *     * The blob owns the content:
 *         * Hold a unique_ptr, return a ref
 *         * Hold a shared_ptr and return a weak_ref
 *           (we hold shared because there's no borrowed_ptr for unique_ptr).
 * * For structured objects (commit, tree, not having blob content):
 *     * The object owns the content:
 *         * Hold a copy, return a unique_ptr to a generated stringstream
 *         * Hold a copy and a stringstream, same as blob owns the content.
 *         * Can't return a weak_ptr if we don't hold a stringstream.
 *     * The store owns the content is probably not relevant since we copy.
 */

/**
 * Holds information of a stored file - name and hash.
 */
struct file_t {
    name_t name;
    hash_t blob_hash;
};

/**
 * Represents a tree - the working status of the whole file system.
 */
struct tree_t {
    std::vector<file_t> files;
};

/**
 * Represents a blob - which is a file.
 */
struct blob_t {
    std::string content;
    // TODO: content should be std::vector<char>;
};

/**
 * Represents a commit.
 * Points to the last commit in order to create a tree.
 */
struct commit_t {
    commit_t(); // Special case for serializer. TODO: delete.
    commit_t(std::string message, hash_t tree_hash);
    commit_t(std::string message, optional_hash parent_hash, hash_t tree_hash);

    std::string message;
    /**
     * The root commit doesn't have a parent hash.
     */
    optional_hash parent_hash;
    hash_t tree_hash;
};


/**
 * We treat all objects the same in Cit.
 * They can all be serialized and saved.
 */
using object_t = boost::variant<blob_t, commit_t, tree_t>;

/**
 * Used for tests, equals means their serialization is equal.
 */
bool operator==(const cit::object_t& obj, const cit::object_t& other);

bool operator==(const cit::file_t& file, const cit::file_t& other);

using optional_blob = boost::optional<blob_t>;
using optional_commit = boost::optional<commit_t>;

/**
 * Used to serialize and deserialize objects.
 * This implementation is simple, could be json / boost::serialize.
 */
class serializer {
public:
    /**
     * Serializes an object.
     */
    static std::string serialize(const object_t&);

    /**
     * Deserializes the templated object.
     * Returns an Optional which will contain the object on success
     * or be empty on failure to deserialize.
     */
    template <typename Object>
    static boost::optional<Object> deserialize(const std::string& serialized);
};

}

#endif
