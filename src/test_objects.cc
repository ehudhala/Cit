#include  <iostream>

#include "gtest/gtest.h"

#include "objects.h"

using namespace cit;

TEST(objects, construction) {
    Object o;
    o = Blob{};
    o = Commit{};
    o = Tree{};
}
