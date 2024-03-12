cmake_minimum_required(VERSION 3.27)
project(RenderEngine C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_DIR Source)
file(GLOB SOURCE_FILES
        ${SOURCE_DIR}/*.c
        ${SOURCE_DIR}/*.h)

if (WIN32)
    set(CMAKE_PREFIX_PATH "C:/Library/SDL2-2.30.1/x86_64-w64-mingw32")
endif (WIN32)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# 'm' - math library.
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} m)