cmake_minimum_required(VERSION 3.16)
project(Gen LANGUAGES C CXX)

if (MSVC OR CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC "${PROJECT_SOURCE_DIR}/src/")
set(DEPS "${PROJECT_SOURCE_DIR}/deps/")

set(JSON_DIR "${DEPS}/json")

file(GLOB_RECURSE PRJ_SRC "${SRC}*.cpp")

add_executable(${PROJECT_NAME}
    ${PRJ_SRC}
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${SRC}
    ${JSON_DIR}
)

