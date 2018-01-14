#pragma once

#include "boost/variant.hpp"

namespace cit {

class Blob {
};

class Commit {
};

class Tree {
};

using Object = boost::variant<Blob, Commit, Tree>;

}
