#include "objects.h"

static cit::serializer_t serializer{};

bool operator==(const cit::object_t& obj, const cit::object_t& other) {
    return serializer.serialize(obj) == serializer.serialize(other);
}
