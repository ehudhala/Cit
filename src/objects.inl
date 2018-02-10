#include <istream>
#include <sstream>
#include <memory>
#include <optional>

#include "objects.h"

namespace cit {

std::shared_ptr<std::istream> blob_t::get_content() const {
    return content;
}

std::shared_ptr<std::istream> commit_t::get_content() const {
    return std::make_shared<std::istringstream>(description);
}

hash_t inmemory_object_store_t::save(std::unique_ptr<object_t> object) {
    hash_t hash = hash_func(*object->get_content());
    objects_map[hash] = std::move(object);
    return hash;
}

std::optional<commit_t> inmemory_object_store_t::load_commit(hash_t hash) const {
    auto obj_it = objects_map.find(hash);
    if (obj_it != objects_map.end()) {
        // TODO: return commit;
        return commit_t{"asdf"};
    }
    return std::nullopt;
}

}
