#ifndef _CIT_WORKINGTREE_H_
#define _CIT_WORKINGTREE_H_

#include <string>
#include <vector>
#include <map>

#include "boost/optional.hpp"

#include "objects.h"

namespace cit {

using name_t = std::string;

namespace inmemory {

/**
 * The working tree mirrors the current working directory.
 * It is used to control it (add, remove files), and to list it.
 * Currently we only support flat directories.
 */
class working_tree_t {
public:
    working_tree_t(std::map<name_t, std::string> working_tree)
        : working_tree(working_tree) {}

    /**
     * Lists the files currently in the working tree.
     */
    std::vector<name_t> list();

    /**
     * Reads a file from the working tree.
     * Returns none if the file doesn't exist.
     */
    boost::optional<const std::string&> read(const name_t&);

    /**
     * Checks whether the working tree contains the given name.
     */
    bool contains(const name_t&);

private:
    std::map<name_t, std::string> working_tree;
};

}
}

#endif
