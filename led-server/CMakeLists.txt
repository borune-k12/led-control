cmake_minimum_required(VERSION 3.1.0)

project(led-server)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)

include_directories(sources)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set (sources
    sources/main.cpp
    sources/led-widget.cpp
    sources/communicator.cpp
    sources/led/led-indicator.cpp
    sources/led/led-base.cpp
    sources/drivers/pipe-driver/pipe-driver.cpp
    sources/drivers/led-driver/led-driver.cpp)

add_executable(${PROJECT_NAME}
    ${sources})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets pthread)

if (${BUILD_TESTS})
    add_subdirectory (tests/googletest)
    set (tests
        tests/tests-main.cpp
        tests/pipe-driver-test.cpp
        sources/drivers/pipe-driver/pipe-driver.cpp
        tests/led-driver-test.cpp
        sources/drivers/led-driver/led-driver.cpp
        sources/led/led-base.cpp
        tests/communicator-test.cpp
        sources/communicator.cpp)

    add_executable("${PROJECT_NAME}-test"
        ${tests})

    target_link_libraries("${PROJECT_NAME}-test" gtest)
endif()






