#include "working_tree.h"
#include "utils.h"

namespace cit {
namespace inmemory {

std::vector<name_t> working_tree_t::list() {
    std::vector<name_t> names;
    names.reserve(working_tree.size());
    std::transform(working_tree.begin(), working_tree.end(), 
            std::back_inserter(names), [](auto name) {return name.first;});
    return names;
}

boost::optional<const std::string&> working_tree_t::read(const name_t& name) {
    return optional_read<name_t, std::string, const std::string&>(working_tree, name);
}

void working_tree_t::write(const name_t& name, std::string content) {
    working_tree[name] = content;
}

bool working_tree_t::contains(const name_t& name) {
    // Read returns a const-ref, so we can safely use its implementation and discard the result.
    return bool(read(name));
}

}

template <typename WorkingTree>
void update_working_tree(WorkingTree& tree, const tree_content_t& new_content) {
    for (const auto& new_file : new_content) {
        tree.write(new_file.first, new_file.second);
    }

    // TODO: review recent commits for unneccesary copies.

    // auto tree_list = tree.list();

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
