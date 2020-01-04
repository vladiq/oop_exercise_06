cmake_minimum_required(VERSION 3.1)

project(lab6)

enable_testing()

set(Figure_source square.cpp)

add_library(figure STATIC ${Figure_source})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(boostTest test.cpp)
target_link_libraries(boostTest ${Boost_LIBRARIES} figure)
add_test(NAME Test1 COMMAND test1)

add_executable(lab6
    main.cpp
)

set_property(TARGET lab6 PROPERTY CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
