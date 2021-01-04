project(truffle_sdl2_wrapper CXX)
add_library(${PROJECT_NAME}
    renderer.cpp
    window.cpp
    font.cpp
    texture.cpp
    font_storage.cpp
    renderer_storage.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    absl::strings
    absl::flat_hash_map
    absl::str_format
)
