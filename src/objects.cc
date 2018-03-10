#include <iostream>

#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/archive_exception.hpp"
#include "boost/serialization/optional.hpp"
#include "boost/serialization/vector.hpp"

#include "objects.h"


namespace cit {

commit_t::commit_t()
    : commit_t("", tree_t{}) {}

commit_t::commit_t(std::string description)
    : commit_t(description, tree_t{}) {}

commit_t::commit_t(std::string description, hash_t parent_hash)
    : commit_t(description, parent_hash, tree_t{}) {}

commit_t::commit_t(std::string description, tree_t tree)
    : description{description}, parent_hash{boost::none}, tree{tree} {}

commit_t::commit_t(std::string description, hash_t parent_hash, tree_t tree)
    : description{description}, parent_hash{parent_hash}, tree{tree} {}

struct object_equals {
    bool operator()(const cit::blob_t& blob, const cit::blob_t& other) const {
        return blob.content == other.content;
    }

    bool operator()(const cit::commit_t& commit, const cit::commit_t& other) const {
        return commit.description == other.description
            && commit.parent_hash == other.parent_hash
            && commit.tree == other.tree;
    }

    bool operator()(const cit::tree_t& tree, const cit::tree_t& other) const {
        auto match = std::mismatch(tree.files.begin(), tree.files.end(),
                other.files.begin(), other.files.end(),
                [](const auto& lhs, const auto& rhs) {
                    return lhs.name == rhs.name && lhs.hash == rhs.hash;
                });
        return match.first == tree.files.end() && match.second == other.files.end();
    }

    template <class Object, class Other>
    bool operator()(const Object&, const Other&) const {
        return false;
    }
};

bool operator==(const cit::object_t& obj, const cit::object_t& other) {
    return boost::apply_visitor(object_equals{}, obj, other);
}

}

namespace boost {
namespace serialization {

template<typename Archive>
void serialize(Archive& ar, cit::blob_t& blob, const unsigned int)
{
    ar & blob.content;
}

template<typename Archive>
void serialize(Archive& ar, cit::commit_t& commit, const unsigned int)
{
    ar & commit.description;
    ar & commit.parent_hash;
    ar & commit.tree;
}

template<typename Archive>
void serialize(Archive& ar, cit::file_t& file, const unsigned int)
{
    ar & file.name;
    ar & file.hash;
}

template<typename Archive>
void serialize(Archive& ar, cit::tree_t& tree, const unsigned int)
{
    ar & tree.files;
}

}
}


namespace cit {

struct serialize_visitor {
    template <typename Object>
    std::string operator()(const Object& object) const {
        std::ostringstream os;
        boost::archive::text_oarchive oa(os);
        oa << object;
        return os.str();
    }
};

std::string serializer::serialize(const object_t& object) {
    return boost::apply_visitor(serialize_visitor{}, object);
}

template <typename Object>
boost::optional<Object> serializer::deserialize(const std::string& serialized) {
    try {
        std::istringstream is(serialized);
        boost::archive::text_iarchive ia(is);
        Object object;
        ia >> object;
        return object;
    }
    catch (boost::archive::archive_exception) {
        return boost::none;
    }
}

// Instantiations of deserialize.
template boost::optional<blob_t> serializer::deserialize<blob_t>(const std::string&);
template boost::optional<commit_t> serializer::deserialize<commit_t>(const std::string&);
template boost::optional<tree_t> serializer::deserialize<tree_t>(const std::string&);

}

