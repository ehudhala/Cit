#include  <iostream>

#include "gtest/gtest.h"

#include "objects.h"

using namespace cit;

std::string read_stream(std::istream& in) {
    std::string s;
    in >> s;
    return s;
}

TEST(blob, returns_content) {
    inmemory_blob_t blob("asdf");
    ASSERT_EQ(blob.content, read_stream(*blob.get_content()));
}

TEST(commit, returns_content) {
    commit_t commit("asdf");
    ASSERT_EQ(commit.description, read_stream(*commit.get_content()));
}

TEST(inmemory_object_store, save) {
    commit_t commit("asdf");
    inmemory_object_store_t objects;
    cit::id_t id = objects.save(commit);
    ASSERT_EQ(id, 1);
}
