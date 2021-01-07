cmake_minimum_required(VERSION 3.17)
project(Truffle)

set(CMAKE_CXX_STANDARD 17)
set(ABSL_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2 REQUIRED)
find_package(absl REQUIRED)
find_package(fmt REQUIRED) # used in absl internally
find_package(spdlog REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(common)
add_subdirectory(engine)
add_subdirectory(object)
add_subdirectory(controller)
add_subdirectory(wrapper/sdl2)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
    truffle_engine
#    truffle_sdl2_wrapper
#    truffle_common
)

