cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
project(Timetabler)

set(Timetabler_VERSION_MAJOR 0)
set(Timetabler_VERSION_MINOR 1)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS -Wno-literal-suffix)

set(OPEN_WBO_PATH "" CACHE PATH "Open-WBO path")
set(CSVPARSER_PATH "" CACHE PATH "CSVParser path")

include_directories(include)
include_directories(${OPEN_WBO_PATH})
include_directories(${OPEN_WBO_PATH}/solvers/minisat2.2)
include_directories(${CSVPARSER_PATH})

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES ${CSVPARSER_PATH}/CSVparser.cpp)

# file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
# list(APPEND TEST_SOURCES ${SOURCES})
# list(REMOVE_ITEM TEST_SOURCES "src/main.cpp")

add_executable(timetabler ${SOURCES})
# add_executable(tests ${TEST_SOURCES})

target_link_libraries(timetabler -L${OPEN_WBO_PATH})
target_link_libraries(timetabler -lopen-wbo -lyaml-cpp)

# target_link_libraries(tests -L${OPEN_WBO_PATH})
# target_link_libraries(tests -lopen-wbo -lyaml-cpp)
# target_link_libraries(tests -lgtest -pthread)

install(TARGETS timetabler DESTINATION bin)