#pragma once

#include <map>

#include "boost/optional.hpp"


template <class Key, class Val, class Return>
boost::optional<Return> optional_read(const std::map<Key, Val>& map, const Key& key) {
    auto it = map.find(key);
    if (it != map.end()) {
        return it->second;
    }
    return boost::none;
}
