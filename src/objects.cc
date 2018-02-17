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

std::string serializer_t::serialize(const object_t& object) {
    return boost::apply_visitor(serialize_visitor{}, object);
}

optional_blob serializer_t::deserialize_blob(const std::string& serialized) {
    return blob_t{serialized};
}

optional_commit serializer_t::deserialize_commit(const std::string& serialized) {
    return commit_t{serialized};
}

}
