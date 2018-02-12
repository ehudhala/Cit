#include "objects.h"

namespace cit {

struct serialize_visitor {
    std::string operator()(const blob_t& blob) const {
        return blob.content;
    }
    std::string operator()(const commit_t& commit) const {
        return commit.description;
    }
};

// TODO: use boost::serialize.
std::string serializer_t::serialize(const object_t& object) const {
    return boost::apply_visitor(serialize_visitor{}, object);
}

optional_blob serializer_t::deserialize_blob(const std::string& serialized) const {
    return blob_t{serialized};
}

optional_commit serializer_t::deserialize_commit(const std::string& serialized) const {
    return commit_t{serialized};
}

}
