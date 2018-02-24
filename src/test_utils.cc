#include "test_utils.h"

using namespace cit;

bool operator==(const cit::object_t& obj, const cit::object_t& other) {
    return cit::serializer_t::serialize(obj) == cit::serializer_t::serialize(other);
}


object_store_t inc_object_store() {
    return inmemory::object_store_t<serializer_t>(incrementing_hash_func{});
}

index_t inc_index() {
    return index_t{object_store_t(inc_object_store())};
}

// index_t inc_index(std::shared_ptr<object_store_t> objects) {
//     return index_t{std::make_shared<object_store_t>(objects)};
// }

// store inc_store() {
//     auto objects{std::make_shared<object_store_t>(inc_object_store())};
//     auto index{inc_index(objects)};
//     return store(std::move(index), std::move(objects));
// }
