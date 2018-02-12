#pragma once
#ifndef _OBJECTSH
#define _OBJECTSH

#include <string>

#include "boost/variant.hpp"
#include "boost/optional.hpp"

namespace cit {

using hash_t = std::size_t;
using ref_t = std::string;

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
struct blob_t {
    std::string content;
};

struct commit_t {
    std::string description;
};

using object_t = boost::variant<blob_t, commit_t>;

using optional_blob = boost::optional<blob_t>;
using optional_commit = boost::optional<commit_t>;

struct serializer_t {
    std::string serialize(const object_t&) const;
    optional_blob deserialize_blob(const std::string& serialized) const;
    optional_commit deserialize_commit(const std::string& serialized) const;
};

}

#endif
