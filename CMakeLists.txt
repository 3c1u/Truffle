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

add_executable(Truffle
  main.cpp
  window.h
  non_copyable.h
  exception.h
  renderer.h
  dispatcher.h
  renderable.h
  texture.h
  logger.h
  scene.h
  scene_manager.h
  button.h
  stateful_object_manager.h
  font.h
  font_storage.h
  color.h
  bus.h
  event.h)

target_include_directories(Truffle PRIVATE ${SPDLOG_INCLUDE_DIRS})
target_link_libraries(Truffle PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} fmt::fmt
         absl::strings absl::flat_hash_map absl::str_format)
