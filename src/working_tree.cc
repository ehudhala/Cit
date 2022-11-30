#include "working_tree.h"
#include "utils.h"

namespace cit {
namespace inmemory {


working_tree_t::working_tree_t(tree_content_t tree_content)
    : tree_content(tree_content) {}

std::vector<name_t> working_tree_t::list() {
    std::vector<name_t> names;
    names.reserve(tree_content.size());
    std::transform(tree_content.begin(), tree_content.end(), 
            std::back_inserter(names), [](auto name) {return name.first;});
    return names;
}

boost::optional<const std::string&> working_tree_t::read(const name_t& name) {
    return optional_read<name_t, std::string, const std::string&>(tree_content, name);
}

void working_tree_t::write(const name_t& name, std::string content) {
    tree_content[name] = content;
}

bool working_tree_t::contains(const name_t& name) {
    // Read returns a const-ref, so we can safely use its implementation and discard the result.
    return bool(read(name));
}

void working_tree_t::remove(const name_t& name) {
    tree_content.erase(name);
}

}

template <typename WorkingTree>
void update_working_tree(WorkingTree& tree, const tree_content_t& new_content) {
    for (const auto& new_file : new_content) {
        tree.write(new_file.first, new_file.second);
    }
    auto deleted_files = diff_deleted(std::move(tree.list()), new_content);
    for (const auto& deleted_file : deleted_files) {
        tree.remove(deleted_file);
    }
}

template void update_working_tree(inmemory::working_tree_t&, const tree_content_t&); 

std::vector<name_t> diff_deleted(std::vector<name_t> old_names, const tree_content_t& new_content) {
    std::vector<name_t> new_names;
    std::transform(new_content.begin(), new_content.end(),
            std::back_inserter(new_names),
            [](const auto& content) {return content.first;});
    std::sort(new_names.begin(), new_names.end());
    std::sort(old_names.begin(), old_names.end());
    // TODO: set_difference is not unique. The names should be unique so it's not a problem.
    std::vector<name_t> diff;
    std::set_difference(old_names.begin(), old_names.end(),
            new_names.begin(), new_names.end(), std::back_inserter(diff));
    return diff;
}

}
