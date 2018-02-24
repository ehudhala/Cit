#include "test_utils.h"

using namespace cit;

bool operator==(const cit::object_t& obj, const cit::object_t& other) {
    return cit::serializer_t::serialize(obj) == cit::serializer_t::serialize(other);
}


object_store_t inc_object_store() {
    return inmemory::object_store_t<serializer_t>(incrementing_hash_func{});
}

index_t inc_index() {
    return index_t{inc_object_store()};
}

store inc_store() {
    auto objects{inc_object_store()};
    auto index{inc_index()};
    return store(std::move(index));
}

repo inc_repo(working_tree_t tree) {
    return repo{inc_store(), tree};
}

