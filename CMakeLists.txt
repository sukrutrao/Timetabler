cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
project(Timetabler LANGUAGES CXX)

include(TimetablerVersion.cmake)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-parentheses -Wno-misleading-indentation")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DTIMETABLERDEBUG")

set(OPEN_WBO_PATH "${CMAKE_SOURCE_DIR}/dependencies/open-wbo" CACHE PATH "Open-WBO path")
set(YAML_CPP_PATH "${CMAKE_SOURCE_DIR}/dependencies/yaml-cpp-yaml-cpp-0.6.2" CACHE PATH "YAML CPP path")
set(PEGTL_PATH "${CMAKE_SOURCE_DIR}/dependencies/PEGTL-2.7.0" CACHE PATH "PEGTL path")
set(ENABLE_TESTS "OFF" CACHE BOOL "Enable Google tests")
set(GTEST_PATH "${CMAKE_SOURCE_DIR}/dependencies/googletest-release-1.8.1" CACHE PATH "GTest path")

if (EXISTS ${GTEST_PATH})
	set(ENABLE_TESTS "ON")
endif ()

include_directories(include)
include_directories(${OPEN_WBO_PATH})
include_directories(${OPEN_WBO_PATH}/solvers/glucose4.1)
include_directories(${YAML_CPP_PATH}/include)
include_directories(${PEGTL_PATH}/include)
if (${ENABLE_TESTS})
	include_directories(${GTEST_PATH}/googletest/include)
endif ()

file(GLOB_RECURSE SOURCES "src/*.cpp")

if (${ENABLE_TESTS})
	include(GoogleTest)
	enable_testing()

	file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
	list(APPEND TEST_SOURCES ${SOURCES})
	get_filename_component(full_path_main_cpp ${CMAKE_SOURCE_DIR}/src/main.cpp ABSOLUTE)
	list(REMOVE_ITEM TEST_SOURCES "${full_path_main_cpp}")
endif ()

add_definitions(-DNSPACE=Glucose)

add_executable(timetabler ${SOURCES})
if (${ENABLE_TESTS})
	add_executable(tests ${TEST_SOURCES})
endif ()

target_link_libraries(timetabler -L${OPEN_WBO_PATH} -L${YAML_CPP_PATH}/build)
target_link_libraries(timetabler -lopen-wbo -lyaml-cpp)

if (${ENABLE_TESTS})
	target_link_libraries(tests -L${OPEN_WBO_PATH} -L${YAML_CPP_PATH}/build -L${GTEST_PATH}/build/googlemock/gtest)
	target_link_libraries(tests -lopen-wbo -lyaml-cpp -lgtest -pthread)
endif ()

install(TARGETS timetabler DESTINATION bin)
if (${ENABLE_TESTS})
	gtest_discover_tests(tests)
endif ()

set(CPACK_PACKAGE_VERSION ${Timetabler_VERSION})

SET(CPACK_GENERATOR "DEB")
SET(CPACK_SOURCE_GENERATOR "TGZ")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "A highly customizable timetabling software for educational institutions that encodes timetabling constraints as a SAT formula and solves them using a MaxSAT solver")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Prateek Kumar <prateek@prateekkumar.in>")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/GoodDeeds/Timetabler")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)

INCLUDE(CPack)
