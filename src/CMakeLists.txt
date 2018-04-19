add_library(working_tree OBJECT working_tree.cc)
add_library(objects OBJECT objects.cc)
add_library(test_utils OBJECT test_utils.cc)

add_library(working_tree_test OBJECT working_tree_test.cc)
add_library(objects_test OBJECT objects_test.cc)
add_library(store_test OBJECT store_test.cc)
add_library(repo_test OBJECT repo_test.cc)

add_executable(cit_tests
        $<TARGET_OBJECTS:working_tree>
        $<TARGET_OBJECTS:working_tree_test>
        $<TARGET_OBJECTS:objects>
        $<TARGET_OBJECTS:objects_test>
        $<TARGET_OBJECTS:store_test>
        $<TARGET_OBJECTS:repo_test>
        $<TARGET_OBJECTS:test_utils>)
target_link_libraries(cit_tests -lgtest -lgtest_main -lpthread -lboost_serialization)
