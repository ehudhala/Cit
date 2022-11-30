#ifndef _CIT_UTILS_H_
#define _CIT_UTILS_H_

#include <map>
#include <vector>

#include "boost/optional.hpp"


template <typename Key, typename Val, typename Return>
boost::optional<Return> optional_read(const std::map<Key, Val>& map, const Key& key) {
    auto it = map.find(key);
    if (it != map.end()) {
        return it->second;
    }
    return boost::none;
}

#endif
