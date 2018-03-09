#ifndef _CIT_TEST_UTILS_H_
#define _CIT_TEST_UTILS_H_

#include "objects.h"
#include "store.h"
#include "repo.h"
#include "working_tree.h"

using namespace cit;

using object_store_t = inmemory::object_store_t<serializer_t>;
using index_t = inmemory::index_t<object_store_t>;
using store = store_t<index_t>;
using repo = repo_t<store, inmemory::working_tree_t>;
using working_tree_t = inmemory::working_tree_t;

/**
 * Used for tests, equals means their serialization is equal.
 */
bool operator==(const cit::object_t& obj, const cit::object_t& other);

struct incrementing_hash_func {
    /**
     * This obviously isn't a real hash function,
     * but it is enough for our tests.
     */
    hash_t hash = 0;
    hash_t operator()(const std::string&) {
        return hash++;
    }
};

/**
 * Creates a new object store, with an incrememnting hash func.
 */
object_store_t inc_object_store();

/**
 * Creates a new index, with a new object store using an incrementing hash func.
 */
index_t inc_index();

/**
 * Creates a new store.
 */
store inc_store();

/**
 * Creates a new repo.
 */
repo inc_repo(working_tree_t);

#endif
