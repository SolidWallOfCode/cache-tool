cmake_minimum_required(VERSION 3.12)

project(CacheTool)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

include(ExternalProject)
set(EP_BASE extern)

ExternalProject_Add(YAML_CPP
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG yaml-cpp-0.6.2
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_HOME_DIRECTORY}
)

find_library(LIB_YAML "libyaml-cpp.a" PATHS "lib")

ExternalProject_Add(LibSWOC
    GIT_REPOSITORY https://github.com/solidwallofcode/libswoc.git
    GIT_TAG bwf-doc
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_HOME_DIRECTORY}
)

add_executable(cache-tool
    include/CacheDefs.h
    include/CacheScan.h
    include/ts-compat.h
    include/tscore/ArgParser.h
    include/tscore/ts_HashContext.h
    include/tscore/ts_CryptoHash.h
    src/CacheDefs.cc
    src/CacheScan.cc
    src/CacheTool.cc
    src/Compatibility.cc
    src/ext/ArgParser.cc
)
add_dependencies(cache-tool LibSWOC YAML_CPP)
target_link_libraries(cache-tool ${CMAKE_HOME_DIRECTORY}/lib/libyaml-cpp.a ${CMAKE_HOME_DIRECTORY}/lib/libswoc++.a ${POSIX_THREAD_LIB})

add_custom_target(clang-format-src COMMAND ${CMAKE_HOME_DIRECTORY}/tools/clang-format.sh ${CMAKE_HOME_DIRECTORY}/src)
add_custom_target(clang-format)
add_dependencies(clang-format clang-format-src)
