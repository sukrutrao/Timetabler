cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
project(Timetabler)

set(Timetabler_VERSION_MAJOR 0)
set(Timetabler_VERSION_MINOR 2)
set(Timetabler_VERSION_PATCH 0)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS -Wno-literal-suffix)

set(OPEN_WBO_PATH "${Timetabler_SOURCE_DIR}/dependencies/open-wbo" CACHE PATH "Open-WBO path")
set(YAML_CPP_PATH "${Timetabler_SOURCE_DIR}/dependencies/yaml-cpp-yaml-cpp-0.6.2" CACHE PATH "YAML CPP path")
set(CSVPARSER_PATH "${Timetabler_SOURCE_DIR}/dependencies/CSVparser" CACHE PATH "CSVParser path")
set(PEGTL_PATH "${Timetabler_SOURCE_DIR}/dependencies/PEGTL-2.2.0" CACHE PATH "PEGTL path")

include_directories(include)
include_directories(${OPEN_WBO_PATH})
include_directories(${OPEN_WBO_PATH}/solvers/minisat2.2)
include_directories(${YAML_CPP_PATH}/include)
include_directories(${CSVPARSER_PATH})
include_directories(${PEGTL_PATH}/include)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES ${CSVPARSER_PATH}/CSVparser.cpp)

# file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
# list(APPEND TEST_SOURCES ${SOURCES})
# list(REMOVE_ITEM TEST_SOURCES "src/main.cpp")

add_executable(timetabler ${SOURCES})
# add_executable(tests ${TEST_SOURCES})

target_link_libraries(timetabler -L${OPEN_WBO_PATH} -L${YAML_CPP_PATH}/build)
target_link_libraries(timetabler -lopen-wbo -lyaml-cpp)

# target_link_libraries(tests -L${OPEN_WBO_PATH})
# target_link_libraries(tests -lopen-wbo -lyaml-cpp)
# target_link_libraries(tests -lgtest -pthread)

install(TARGETS timetabler DESTINATION bin)

set(CPACK_PACKAGE_VERSION_MAJOR ${Timetabler_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Timetabler_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Timetabler_VERSION_PATCH})

SET(CPACK_GENERATOR "DEB")
SET(CPACK_SOURCE_GENERATOR "TGZ")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Encode timetabling constraints as a SAT formula and solve using a MaxSAT solver to generate a Timetable for IITH")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Prateek Kumar <prateek@prateekkumar.in>")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/GoodDeeds/Timetabler")
set(CPACK_RESOURCE_FILE_LICENSE ${Timetabler_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${Timetabler_SOURCE_DIR}/README.md)
set(CPACK_DEBIAN_RESOURCE_FILE_CHANGELOG ${Timetabler_SOURCE_DIR}/debian/changelog)

INCLUDE(CPack)
