#pragma once
#ifndef _OBJECTSH
#define _OBJECTSH

#include <map>
#include <string>
#include <functional>
#include <type_traits>

#include "boost/variant.hpp"

namespace cit {

using hash_t = std::size_t;
using ref_t = std::string;

class object_t {
public:
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
    virtual std::shared_ptr<std::istream> get_content() const = 0;

    virtual ~object_t() {}
};

class blob_t : public object_t {
public:
    blob_t(std::shared_ptr<std::istream> content) : content(std::move(content)) {}
    std::shared_ptr<std::istream> get_content() const;

private:
    std::shared_ptr<std::istream> content;
};

class commit_t : public object_t {
public:
    commit_t(std::string description) : description(description) {}
    std::shared_ptr<std::istream> get_content() const;

    std::string description;
};

template <class commit_t>
class inmemory_object_store_t {
public:
    inmemory_object_store_t(std::function<hash_t(object_t&)> hash_func)
        : hash_func(hash_func) {}

    hash_t save(std::unique_ptr<object_t>);
    commit_t load_commit(hash_t) const;
private:
    std::map<hash_t, std::unique_ptr<object_t>> objects_map;
    std::function<hash_t(object_t&)> hash_func;
};

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
#include "objects.inl"
