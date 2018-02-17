#include "objects.h"

bool operator==(const cit::object_t& obj, const cit::object_t& other) {
    return cit::serializer_t::serialize(obj) == cit::serializer_t::serialize(other);
}
