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

include_directories(${CMAKE_SOURCE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

add_subdirectory(common)
add_subdirectory(engine)
add_subdirectory(object)
add_subdirectory(wrapper/sdl2)

add_executable(Truffle main.cpp)
target_link_libraries(Truffle PRIVATE truffle_engine)

#add_executable(Truffle
#  main.cpp
#  window.h
#        renderer.h
#  renderable.h
#  texture.h
#  logger.h
#  object_tree.h
#  button.h
#        font_storage.h
#  color.h
#  bus.h
#  event.h)
#
#target_include_directories(Truffle PRIVATE ${SPDLOG_INCLUDE_DIRS})
#target_link_libraries(Truffle PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} fmt::fmt
#         absl::strings absl::flat_hash_map absl::str_format)
