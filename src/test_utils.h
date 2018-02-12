#include "objects.h"

static cit::serializer_t serializer{};

/**
 * Used for tests, equals means their serialization is equal.
 */
bool operator==(const cit::object_t& obj, const cit::object_t& other);
