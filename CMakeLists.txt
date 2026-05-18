cmake_minimum_required(VERSION 3.24)

project(test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(SFML)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frontend/app.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frontend/ui_logic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frontend/ui_render.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/ship.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/board.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/game.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/board.h
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/Coordinate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/game.h
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/ship.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frontend/app.h
    ${CMAKE_CURRENT_SOURCE_DIR}/frontend/ui_logic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/frontend/ui_render.h
    ${CMAKE_CURRENT_SOURCE_DIR}/frontend/ui_types.h
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY
                                                 ${CMAKE_CURRENT_SOURCE_DIR}/build)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/backend
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    SFML::Graphics
)

if(ENABLE_DEBUG)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g)
endif()

add_custom_target(
    clang-format
    COMMAND clang-format
    --dry-run
    -Werror
    ${SOURCES}
    ${HEADERS}
)
