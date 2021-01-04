project(truffle_object CXX)
add_library(${PROJECT_NAME}
    image.cpp
    text.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${SDL2_LIBRARIES}
    absl::strings
    absl::flat_hash_map
    absl::str_format
)
