project(truffle_engine CXX)
add_library(${PROJECT_NAME}
    controller.cpp
    dispatcher.cpp
    scene_manager.cpp
    engine.cpp
    object.cpp
    scene.cpp
    router.cpp
    actor_table.cpp
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
