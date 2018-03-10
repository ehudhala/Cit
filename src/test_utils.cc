#include "test_utils.h"

using namespace cit;

struct object_equals {
    bool operator()(const cit::blob_t& blob, const cit::blob_t& other) const {
        return blob.content == other.content;
    }

    bool operator()(const cit::commit_t& commit, const cit::commit_t& other) const {
        return commit.description == other.description
            && commit.parent_hash == other.parent_hash;
    }

    bool operator()(const cit::tree_t& tree, const cit::tree_t& other) const {
        auto match = std::mismatch(tree.files.begin(), tree.files.end(),
                other.files.begin(), other.files.end(),
                [](const auto& lhs, const auto& rhs) {
                    return lhs.name == rhs.name && lhs.hash == rhs.hash;
                });
        return match.first == tree.files.end() && match.second == other.files.end();
    }

    template <class Object, class Other>
    bool operator()(const Object&, const Other&) const {
        return false;
    }
};

bool operator==(const cit::object_t& obj, const cit::object_t& other) {
    return boost::apply_visitor(object_equals{}, obj, other);
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
    return store(std::move(index));
}

repo inc_repo(working_tree_t tree) {
    return repo{inc_store(), tree};
}

