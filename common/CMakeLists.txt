project(truffle_common CXX)
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${SPDLOG_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})
