#include <iostream>

#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/archive_exception.hpp"
#include "boost/serialization/optional.hpp"
#include "boost/serialization/vector.hpp"

#include "objects.h"


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
}

template<typename Archive>
void serialize(Archive& ar, cit::tree_entry& entry, const unsigned int)
{
    ar & entry.name;
    ar & entry.hash;
}

template<typename Archive>
void serialize(Archive& ar, cit::tree_t& tree, const unsigned int)
{
    ar & tree.files;
}

}
}

namespace cit {

commit_t::commit_t()
    : description{""}, parent_hash{boost::none} {}

commit_t::commit_t(std::string description)
    : description{description}, parent_hash{boost::none} {}

commit_t::commit_t(std::string description, hash_t parent_hash)
    : description{description}, parent_hash{parent_hash} {}

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

