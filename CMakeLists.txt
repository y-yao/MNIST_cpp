cmake_minimum_required(VERSION 2.8)
enable_language(CXX)

# ctest configs
include(CTest)
enable_testing()

# pedantic option
option(PEDANTIC "PEDANTIC" ON)

# Set a default build type if none was specified
set(default_build_type "Debug")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Flags for G++
if (PEDANTIC)
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -pedantic -Wextra -std=c++11")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c++11")
endif (PEDANTIC)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g --coverage")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
option(TEST_COVERAGE "Coverage testing with gcov" OFF)

if(TEST_COVERAGE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif(TEST_COVERAGE)

# Set this PA's name
set(PA_NAME "pa5-sys")

# Path to this PA's source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(SCRIPT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/scripts")

# Include path to this PA's header files
include_directories(${SRC_DIR})

#-------------------------------------------------------------------------
# Datasets
#-------------------------------------------------------------------------
# These come from the online website from MNIST

add_custom_target("training-images.bin"
                  COMMAND test "!" "-e" "training-images.bin" "&&" wget "http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz" "||" echo "done"
                  COMMAND test "!" "-e" "training-images.bin" "&&" mv "train-images-idx3-ubyte.gz" "training-images.bin.gz" "||" echo "done"
                  COMMAND test "!" "-e" "training-images.bin" "&&" gzip "-df" "training-images.bin.gz" "||" echo "done")

add_custom_target("training-labels.bin"
                  COMMAND test "!" "-e" "training-labels.bin" "&&" wget "http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz" "||" echo "done"
                  COMMAND test "!" "-e" "training-labels.bin" "&&" mv "train-labels-idx1-ubyte.gz" "training-labels.bin.gz" "||" echo "done"
                  COMMAND test "!" "-e" "training-labels.bin" "&&" gzip "-df" "training-labels.bin.gz" "||" echo "done")

add_custom_target("inference-images.bin"
                  COMMAND test "!" "-e" "inference-images.bin" "&&" wget "http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz" "||" echo "done"
                  COMMAND test "!" "-e" "inference-images.bin" "&&" mv "t10k-images-idx3-ubyte.gz" "inference-images.bin.gz" "||" echo "done"
                  COMMAND test "!" "-e" "inference-images.bin" "&&" gzip "-df" "inference-images.bin.gz" "||" echo "done")

add_custom_target("inference-labels.bin"
                  COMMAND test "!" "-e" "inference-labels.bin" "&&" wget "http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz" "||" echo "done"
                  COMMAND test "!" "-e" "inference-labels.bin" "&&" mv "t10k-labels-idx1-ubyte.gz" "inference-labels.bin.gz" "||" echo "done"
                  COMMAND test "!" "-e" "inference-labels.bin" "&&" gzip "-df" "inference-labels.bin.gz" "||" echo "done")

set(BINS         "training-images.bin")
set(BINS ${BINS} "training-labels.bin")
set(BINS ${BINS} "inference-images.bin")
set(BINS ${BINS} "inference-labels.bin")

# Tiny datasets (10)

add_custom_target("tiny-training-images-10.bin"
                  COMMAND "${SCRIPT_DIR}/gen-mnist-datasets" "--labels" "training-labels.bin" "--images" "training-images.bin" "--size" "10" "--out" "tiny"
                  DEPENDS "training-labels.bin" "training-images.bin")
add_custom_target("tiny-training-labels-10.bin"
                  DEPENDS "tiny-training-images-10.bin")

add_custom_target("tiny-inference-images-10.bin"
                  COMMAND "${SCRIPT_DIR}/gen-mnist-datasets" "--labels" "inference-labels.bin" "--images" "inference-images.bin" "--size" "10" "--out" "tiny"
                  DEPENDS "inference-labels.bin" "inference-images.bin")

add_custom_target("tiny-inference-labels-10.bin"
                  DEPENDS "tiny-inference-images-10.bin")

set(BINS ${BINS} "tiny-training-images-10.bin")
set(BINS ${BINS} "tiny-training-labels-10.bin")
set(BINS ${BINS} "tiny-inference-images-10.bin")
set(BINS ${BINS} "tiny-inference-labels-10.bin")

# Small datasets (1000)

add_custom_target("small-training-images-1000.bin"
                  COMMAND "${SCRIPT_DIR}/gen-mnist-datasets" "--labels" "training-labels.bin" "--images" "training-images.bin" "--size" "1000" "--out" "small"
                  DEPENDS "training-labels.bin" "training-images.bin")
add_custom_target("small-training-labels-1000.bin"
                  DEPENDS "small-training-images-1000.bin")

add_custom_target("small-inference-images-1000.bin"
                  COMMAND "${SCRIPT_DIR}/gen-mnist-datasets" "--labels" "inference-labels.bin" "--images" "inference-images.bin" "--size" "1000" "--out" "small"
                  DEPENDS "inference-labels.bin" "inference-images.bin")

add_custom_target("small-inference-labels-1000.bin"
                  DEPENDS "small-inference-images-1000.bin")

set(BINS ${BINS} "small-training-images-1000.bin")
set(BINS ${BINS} "small-training-labels-1000.bin")
set(BINS ${BINS} "small-inference-images-1000.bin")
set(BINS ${BINS} "small-inference-labels-1000.bin")

#-------------------------------------------------------------------------
# Shared libraries
#-------------------------------------------------------------------------

add_library(types SHARED "${SRC_DIR}/types.cc" "${SRC_DIR}/types.h" "${SRC_DIR}/types.inl" "${SRC_DIR}/constants.h")
add_library(utils SHARED "${SRC_DIR}/utils.cc" "${SRC_DIR}/utils.h" "${SRC_DIR}/types.h" "${SRC_DIR}/types.inl")
add_library(hrs-ones SHARED "${SRC_DIR}/hrs-ones.cc" "${SRC_DIR}/hrs-ones.h" "${SRC_DIR}/types.h" "${SRC_DIR}/types.inl")
add_library(hrs-knn-brute SHARED "${SRC_DIR}/hrs-knn-brute.cc" "${SRC_DIR}/hrs-knn-brute.h" "${SRC_DIR}/types.h" "${SRC_DIR}/types.inl")
add_library(hrs-knn-kdtree SHARED "${SRC_DIR}/hrs-knn-kdtree.cc" "${SRC_DIR}/hrs-knn-kdtree.h" "${SRC_DIR}/types.h" "${SRC_DIR}/types.inl" "${SRC_DIR}/kdtree.h" "${SRC_DIR}/kdtree.inl")
add_library(hrs-alt SHARED "${SRC_DIR}/hrs-alt.cc" "${SRC_DIR}/hrs-alt.h" "${SRC_DIR}/types.h" "${SRC_DIR}/types.inl")

#-------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------

# common variables

set(MEMTEST_DUMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/memtest-logs/")

# types-test

add_executable(types-test "${TEST_DIR}/types-test.cc")
target_link_libraries(types-test "types")
add_test(types-test types-test)
set(TESTS "types-test")
add_custom_target(check-types
                  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/types-test"
                  DEPENDS types-test)
set(MEMTESTS "memcheck-types")
add_custom_target(memcheck-types
                  COMMAND mkdir -p "memtest-logs"
                  COMMAND echo "---memcheck: types---"
                  COMMAND valgrind "--trace-children=yes"
                                   "--leak-check=full"
                                   "--error-exitcode=1"
                                   "--log-file=${MEMTEST_DUMP_DIR}/types-test.log"
                                   "--undef-value-errors=no"
                                   "${CMAKE_CURRENT_BINARY_DIR}/types-test"
                                   ">${MEMTEST_DUMP_DIR}/types-test.stdout"
                  DEPENDS types-test)

# utils-test

add_executable(utils-test "${TEST_DIR}/utils-test.cc")
add_dependencies(utils-test ${BINS})
target_link_libraries(utils-test "types" "utils")
add_test(utils-test utils-test)
set(TESTS ${TESTS} "utils-test")
add_custom_target(check-utils
                  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/utils-test"
                  DEPENDS utils-test)
set(MEMTESTS ${MEMTESTS} "memcheck-utils")
add_custom_target(memcheck-utils
                  COMMAND mkdir -p "memtest-logs"
                  COMMAND echo "---memcheck: utils---"
                  COMMAND valgrind "--trace-children=yes"
                                   "--leak-check=full"
                                   "--error-exitcode=1"
                                   "--log-file=${MEMTEST_DUMP_DIR}/utils-test.log"
                                   "--undef-value-errors=no"
                                   "${CMAKE_CURRENT_BINARY_DIR}/utils-test"
                                   ">${MEMTEST_DUMP_DIR}/utils-test.stdout"
                  DEPENDS utils-test)

# kdtree-test

add_executable(kdtree-test "${TEST_DIR}/kdtree-test.cc" "${TEST_DIR}/kdtree-test-misc.h" "${SRC_DIR}/kdtree.h" "${SRC_DIR}/kdtree.inl")
add_test(kdtree-test kdtree-test)
set(TESTS ${TESTS} "kdtree-test")
add_custom_target(check-kdtree
                  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/kdtree-test"
                  DEPENDS kdtree-test)
set(MEMTESTS ${MEMTESTS} "memcheck-kdtree")
add_custom_target(memcheck-kdtree
                  COMMAND mkdir -p "memtest-logs"
                  COMMAND echo "---memcheck: kdtree---"
                  COMMAND valgrind "--trace-children=yes"
                                   "--leak-check=full"
                                   "--error-exitcode=1"
                                   "--log-file=${MEMTEST_DUMP_DIR}/kdtree-test.log"
                                   "--undef-value-errors=no"
                                   "${CMAKE_CURRENT_BINARY_DIR}/kdtree-test"
                                   ">${MEMTEST_DUMP_DIR}/kdtree-test.stdout"
                  DEPENDS kdtree-test)

# hrs-ones-test

add_executable(hrs-ones-test "${TEST_DIR}/hrs-ones-test.cc")
add_dependencies(hrs-ones-test ${BINS})
target_link_libraries(hrs-ones-test "types" "utils" "hrs-ones")
add_test(hrs-ones-test hrs-ones-test)
set(TESTS ${TESTS} "hrs-ones-test")
add_custom_target(check-hrs-ones
                  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/hrs-ones-test"
                  DEPENDS hrs-ones-test)
set(MEMTESTS ${MEMTESTS} "memcheck-hrs-ones")
add_custom_target(memcheck-hrs-ones
                  COMMAND mkdir -p "memtest-logs"
                  COMMAND echo "---memcheck: hrs-ones---"
                  COMMAND valgrind "--trace-children=yes"
                                   "--leak-check=full"
                                   "--error-exitcode=1"
                                   "--log-file=${MEMTEST_DUMP_DIR}/hrs-ones-test.log"
                                   "--undef-value-errors=no"
                                   "${CMAKE_CURRENT_BINARY_DIR}/hrs-ones-test"
                                   ">${MEMTEST_DUMP_DIR}/hrs-ones-test.stdout"
                  DEPENDS hrs-ones-test)

# hrs-knn-brute-test

add_executable(hrs-knn-brute-test "${TEST_DIR}/hrs-knn-brute-test.cc")
add_dependencies(hrs-knn-brute-test ${BINS})
target_link_libraries(hrs-knn-brute-test "types" "utils" "hrs-knn-brute")
add_test(hrs-knn-brute-test hrs-knn-brute-test)
set(TESTS ${TESTS} "hrs-knn-brute-test")
add_custom_target(check-hrs-knn-brute
                  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/hrs-knn-brute-test"
                  DEPENDS hrs-knn-brute-test)
set(MEMTESTS ${MEMTESTS} "memcheck-hrs-knn-brute")
add_custom_target(memcheck-hrs-knn-brute
                  COMMAND mkdir -p "memtest-logs"
                  COMMAND echo "---memcheck: hrs-knn-brute---"
                  COMMAND valgrind "--trace-children=yes"
                                   "--leak-check=full"
                                   "--error-exitcode=1"
                                   "--log-file=${MEMTEST_DUMP_DIR}/hrs-knn-brute-test.log"
                                   "--undef-value-errors=no"
                                   "${CMAKE_CURRENT_BINARY_DIR}/hrs-knn-brute-test"
                                   ">${MEMTEST_DUMP_DIR}/hrs-knn-brute-test.stdout"
                  DEPENDS hrs-knn-brute-test)

# hrs-knn-kdtree-test

add_executable(hrs-knn-kdtree-test "${TEST_DIR}/hrs-knn-kdtree-test.cc")
add_dependencies(hrs-knn-kdtree-test ${BINS})
target_link_libraries(hrs-knn-kdtree-test "types" "utils" "hrs-knn-kdtree")
add_test(hrs-knn-kdtree-test hrs-knn-kdtree-test)
set(TESTS ${TESTS} "hrs-knn-kdtree-test")
add_custom_target(check-hrs-knn-kdtree
                  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/hrs-knn-kdtree-test"
                  DEPENDS hrs-knn-kdtree-test)
set(MEMTESTS ${MEMTESTS} "memcheck-hrs-knn-kdtree")
add_custom_target(memcheck-hrs-knn-kdtree
                  COMMAND mkdir -p "memtest-logs"
                  COMMAND echo "---memcheck: hrs-knn-kdtree---"
                  COMMAND valgrind "--trace-children=yes"
                                   "--leak-check=full"
                                   "--error-exitcode=1"
                                   "--log-file=${MEMTEST_DUMP_DIR}/hrs-knn-kdtree-test.log"
                                   "--undef-value-errors=no"
                                   "${CMAKE_CURRENT_BINARY_DIR}/hrs-knn-kdtree-test"
                                   ">${MEMTEST_DUMP_DIR}/hrs-knn-kdtree-test.stdout"
                  DEPENDS hrs-knn-kdtree-test)

# hrs-alt-test

add_executable(hrs-alt-test "${TEST_DIR}/hrs-alt-test.cc")
add_dependencies(hrs-alt-test ${BINS})
target_link_libraries(hrs-alt-test "types" "utils" "hrs-alt")
add_test(hrs-alt-test hrs-alt-test)
set(TESTS ${TESTS} "hrs-alt-test")
add_custom_target(check-hrs-alt
                  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/hrs-alt-test"
                  DEPENDS hrs-alt-test)
set(MEMTESTS ${MEMTESTS} "memcheck-hrs-alt")
add_custom_target(memcheck-hrs-alt
                  COMMAND mkdir -p "memtest-logs"
                  COMMAND echo "---memcheck: hrs-alt---"
                  COMMAND valgrind "--trace-children=yes"
                                   "--leak-check=full"
                                   "--error-exitcode=1"
                                   "--log-file=${MEMTEST_DUMP_DIR}/hrs-alt-test.log"
                                   "--undef-value-errors=no"
                                   "${CMAKE_CURRENT_BINARY_DIR}/hrs-alt-test"
                                   ">${MEMTEST_DUMP_DIR}/hrs-alt-test.stdout"
                  DEPENDS hrs-alt-test)

#-------------------------------------------------------------------------
# Eval targets
#-------------------------------------------------------------------------
add_executable(hrs-eval "${SRC_DIR}/hrs-eval.cc")
add_dependencies(hrs-eval ${BINS})
target_link_libraries(hrs-eval "hrs-ones" "hrs-knn-brute" "hrs-knn-kdtree" "hrs-alt" "types" "utils")

#-------------------------------------------------------------------------
# Global targets
#-------------------------------------------------------------------------

add_custom_target(check
                  COMMAND echo "Running all unit tests ..."
                  COMMAND ctest "--timeout" "120"
                  DEPENDS ${TESTS})

add_custom_target(memcheck)
add_dependencies(memcheck ${MEMTESTS})

add_custom_target(eval
                  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/hrs-eval"
                  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/hrs-eval" "2"
                  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/hrs-eval" "4"
                  DEPENDS hrs-eval)

add_custom_target(coverage
                  COMMAND lcov --capture --directory . --output-file coverage.info
                  COMMAND genhtml coverage.info --output-directory coverage-html
                  DEPENDS check)
