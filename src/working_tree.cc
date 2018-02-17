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

}
}
