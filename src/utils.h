#ifndef _CIT_UTILS_H_
#define _CIT_UTILS_H_

#include <map>
#include <vector>

#include "boost/optional.hpp"

#include "objects.h"

template <typename Key, typename Val, typename Return>
boost::optional<Return> optional_read(const std::map<Key, Val>& map, const Key& key) {
    auto it = map.find(key);
    if (it != map.end()) {
        return it->second;
    }
    return boost::none;
}

struct failing_deserializtion {
    static std::string serialize(const cit::object_t&) {return "";}
    template <class Object>
    static boost::optional<Object> deserialize(const std::string&) {
        return boost::none;
    }
};

#endif
