#ifndef _CIT_WORKINGTREE_H_
#define _CIT_WORKINGTREE_H_

#include <string>
#include <vector>
#include <set>
#include <map>

#include "boost/optional.hpp"

namespace cit {

using name_t = std::string;
using tree_content_t = std::map<name_t, std::string>;

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
     * Writes new content to the given file.
     * Creates the file if it doesn't exist yet.
     */
    void write(const name_t&, std::string content);

    /**
     * Checks whether the working tree contains the given name.
     */
    bool contains(const name_t&);

private:
    tree_content_t working_tree;
};

}

/**
 * Updates the given working tree content according to a diff:
 * 1. Creates all the new files
 * 2. Deletes all the deleted files
 * 3. Updates the content of all the changed files.
 */
template <typename WorkingTree>
void update_working_tree(WorkingTree& tree, const tree_content_t&);

}

#endif
