project(truffle_controller CXX)
add_library(${PROJECT_NAME}
    fps.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${SDL2_LIBRARIES}
    absl::strings
    absl::flat_hash_map
    absl::str_format
    truffle_common
    truffle_sdl2_wrapper
)
