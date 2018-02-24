#include <iostream>

#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/archive_exception.hpp"

#include "objects.h"


namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive& ar, cit::blob_t& blob, const unsigned int)
{
    ar & blob.content;
}

template<class Archive>
void serialize(Archive& ar, cit::commit_t& commit, const unsigned int)
{
    ar & commit.description;
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
    template <class object_t>
    std::string operator()(const object_t& object) const {
        std::ostringstream os;
        boost::archive::text_oarchive oa(os);
        oa << object;
        return os.str();
    }
};

std::string serializer_t::serialize(const object_t& object) {
    return boost::apply_visitor(serialize_visitor{}, object);
}

template <class Object>
boost::optional<Object> serializer_t::deserialize(const std::string& serialized) {
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


optional_commit serializer_t::deserialize_commit(const std::string& serialized) {
    return deserialize<commit_t>(serialized);
}

optional_blob serializer_t::deserialize_blob(const std::string& serialized) {
    return deserialize<blob_t>(serialized);
}


}

