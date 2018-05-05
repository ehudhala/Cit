#include <algorithm>

#include "test_utils.h"

using namespace cit;

boost::optional<file_t> find_file(const std::vector<file_t>& files, const name_t& name) {
    auto res = std::find_if(files.begin(), files.end(),
            [&name](const file_t& file) {return file.name == name;});
    if (res == files.end()) {
        return boost::none;
    }
    return *res;
}

object_store_t inc_object_store() {
    return inmemory::object_store_t<serializer>(incrementing_hash_func{});
}

index_t inc_index() {
    return index_t{inc_object_store()};
}

store inc_store() {
    auto objects{inc_object_store()};
    auto index{inc_index()};
    inmemory::ref_store_t refs;
    return store(std::move(index), std::move(refs));
}

repo inc_repo(working_tree_t tree) {
    return repo{inc_store(), tree};
}

repo added_inc_repo(working_tree_t tree) {
    auto r = inc_repo(tree);
    for (auto file_name : tree.list()) {
        r.add(file_name);
    }
    return r;
}

