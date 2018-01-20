#pragma once
#ifndef _OBJECTSH
#define _OBJECTSH

#include <map>
#include <string>

#include "boost/variant.hpp"

namespace cit {

using id_t = std::size_t;
using ref_t = std::string;

class object_t {
public:
    virtual std::unique_ptr<std::istream> get_content() const = 0;
};

class inmemory_blob_t : public object_t {
public:
    inmemory_blob_t(std::string content) : content(content) {}
    std::unique_ptr<std::istream> get_content() const;

    std::string content;
};

class commit_t : public object_t {
public:
    commit_t(std::string description) : description(description) {}
    std::unique_ptr<std::istream> get_content() const;

    std::string description;
};

class inmemory_object_store_t {
public:
    id_t save(const object_t&);
    commit_t load_commit(id_t) const;
private:
    std::map<id_t, object_t&> objects_map;
};

using blob_names_t = std::map<std::string, id_t>;

template <class object_store_t, class blob_t>
class inmemory_index_t {
public:
    inmemory_index_t(object_store_t&);
    id_t add(const std::string& name, blob_t);
private:
    blob_names_t blob_names;
    object_store_t& object_store;
};

class inmemory_ref_store_t {
public:
    id_t get_id(const ref_t&);
    id_t set_ref(const ref_t&, id_t);

    ref_t get_head();
    void set_head(const ref_t&);
};

template <
    template <class, class>class index_t,
    class object_store_t, class ref_store_t,
    class blob_t, class id_func>
class store_t {
public:
    index_t<object_store_t, blob_t> index;
    object_store_t objects;
    ref_store_t refs;
};

}

#endif
#include "objects.inl"
